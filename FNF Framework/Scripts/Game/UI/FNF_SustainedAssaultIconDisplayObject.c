//------------------------------------------------------------------------------------------------
/*!
	This object serves as a wrapper for individual HUD objective widgets.
*/
class FNF_SustainedAssaultIconDisplayObject
{
	//! Topmost widget of this object
	protected Widget m_wRoot;
	//! Text that displays name or symbol of the objective
	protected TextWidget m_wNameText;

	//! The backdrop image of this element
	protected ImageWidget m_wBackdropIcon;
	protected SliderWidget m_wSlider;

	protected VerticalLayoutWidget m_wLayout;

	//! Blend progress of when within the area
	protected float m_fBlendScreenPosition;
	//! Position blending rate
	protected const float POSITION_BLEND_SPEED = 2.5;

	// Initial values
	protected float m_fOriginalXSize;
	protected float m_fOriginalYSize;
	protected float m_fOriginalTextSize;

	// Loaded texture indices
	protected const int ATLAS_INDEX_NEUTRAL = 0;
	protected const int ATLAS_INDEX_BLUFOR = 1;
	protected const int ATLAS_INDEX_OPFOR = 2;


	//! The area this object represents
	protected FNF_SustainedAssaultCapturePoint m_pAffiliatedArea;

	//------------------------------------------------------------------------------------------------
	/*!
		Returns the area this element represents.
	*/
	FNF_SustainedAssaultCapturePoint GetArea()
	{
		return m_pAffiliatedArea;
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Returns root widget of this element.
	*/
	Widget GetRootWidget()
	{
		return m_wRoot;
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Create new wrapper for objective display.
	*/
	void FNF_SustainedAssaultIconDisplayObject(notnull Widget root, notnull FNF_SustainedAssaultCapturePoint area)
	{
		m_wRoot = root;
		m_pAffiliatedArea = area;
		m_wNameText = TextWidget.Cast(m_wRoot.FindAnyWidget("Name"));

		m_wBackdropIcon = ImageWidget.Cast(m_wRoot.FindAnyWidget("Icon_Backdrop"));

		m_wLayout = VerticalLayoutWidget.Cast(m_wRoot.FindAnyWidget("VerticalLayout"));
		m_fOriginalXSize = FrameSlot.GetSizeX(m_wLayout);
		m_fOriginalYSize = FrameSlot.GetSizeY(m_wLayout);
		m_wSlider = SliderWidget.Cast(m_wRoot.FindAnyWidget("Slider"));

		m_fOriginalTextSize= 30;
	}

	//------------------------------------------------------------------------------------------------
	void ~FNF_SustainedAssaultIconDisplayObject()
	{
	}

	//------------------------------------------------------------------------------------------------
	/*
		Update image state according to provided area state
	*/
	protected void UpdateImageState(Widget root, float timeSlice)
	{
		Faction owningFaction = m_pAffiliatedArea.GetCurrentTimedFactionOwner();

		Color ownerColor;
		if (!owningFaction)
			ownerColor = Color.FromRGBA(104, 104, 104, 255);
		else
			ownerColor = owningFaction.GetFactionColor();

		m_wBackdropIcon.SetColor(ownerColor);

		Faction contestingFaction = m_pAffiliatedArea.GetAttackingFaction();
		Color attackerColor;
		if (!contestingFaction)
			attackerColor = Color.FromRGBA(104, 104, 104, 255);
		else
			attackerColor = contestingFaction.GetFactionColor();
		m_wSlider.SetColor(attackerColor);
		
		m_wSlider.SetCurrent(Math.Round(m_pAffiliatedArea.GetCurrentOwnedPercent()));
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Update this widget as static UI element, ie. not reprojecting it automatically.
	*/
	void UpdateStatic(float timeSlice)
	{
		UpdateImageState(m_wRoot, timeSlice);
		// Set objective symbol
		m_wNameText.SetText(m_pAffiliatedArea.GetPointName());
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Update this widget as dynamic UI element projected to screen space
	*/
	void UpdateDynamic(IEntity playerEntity, float timeSlice)
	{
		vector objectiveWorldPosition = m_pAffiliatedArea.GetWorldObjectiveCenter();
		vector projectedScreenPosition = m_wRoot.GetWorkspace().ProjWorldToScreen(objectiveWorldPosition, m_pAffiliatedArea.GetWorld());

		vector cameraMatrix[4];
		m_pAffiliatedArea.GetWorld().GetCurrentCamera(cameraMatrix);

		float alpha = 1.0;
		bool visible = true;

		// Center to screen and stay fixed
		if (playerEntity && m_pAffiliatedArea.IsCharacterInside(SCR_ChimeraCharacter.Cast(playerEntity)))
		{
			// Update screen space blend
			m_fBlendScreenPosition += (timeSlice * POSITION_BLEND_SPEED);
			visible = true; // Always draw inside
		}
		else
		{
			// Worldprojection
			vector dirToCamera = (objectiveWorldPosition - cameraMatrix[3]).Normalized();
			float dot = vector.Dot(dirToCamera, cameraMatrix[2]);
			if (dot < 0.6666 && m_fBlendScreenPosition <= 0) // Force animation
				visible = false;

			float alphaScale = Math.InverseLerp(0.667, 1.0, dot);
			alphaScale = Math.Clamp(alphaScale, 0.0, 1.0);

			if (m_fBlendScreenPosition <= 0.0)
				alpha = Math.Lerp(0.5, 1.0, alphaScale);
			else
				alpha = 1.0;

			// Blend the point out significantly in optics, because the projection is misaligned
			if (ArmaReforgerScripted.IsScreenPointInPIPSights(projectedScreenPosition, ArmaReforgerScripted.GetCurrentPIPSights()))
				alpha = Math.Min(alpha, 0.65);

			m_fBlendScreenPosition -= (timeSlice * POSITION_BLEND_SPEED);
		}

		// Clamp the blend so we don't overshoot
		m_fBlendScreenPosition = Math.Clamp(m_fBlendScreenPosition, 0.0, 1.0);

		// If not visible, do not draw and that's it
		if (!visible)
		{
			m_wRoot.SetVisible(false);
			return;
		}

		// Otherwise update widget

		// Update image state
		UpdateImageState(m_wRoot, timeSlice);

		// Opacity
		m_wRoot.SetOpacity(alpha);

		float x, y;
		float scale = 1.0;
		float textScale = 1.0;
		// Interpolate position
		if (m_fBlendScreenPosition > 0.0)
		{
			WorkspaceWidget ww = m_wRoot.GetWorkspace();
			int w = ww.GetWidth();
			int h = ww.GetHeight();
			float fixedX = ww.DPIUnscale(0.5 * w);
			float fixedY = ww.DPIUnscale(0.15 * h);

			SCR_EditorManagerEntity editorManager = SCR_EditorManagerEntity.GetInstance();
			if (editorManager && editorManager.IsOpened())
				fixedY += 40;

			x = Math.Lerp(projectedScreenPosition[0], fixedX, m_fBlendScreenPosition);
			y = Math.Lerp(projectedScreenPosition[1], fixedY, m_fBlendScreenPosition);
			scale = m_fBlendScreenPosition * 2.25;
			textScale = m_fBlendScreenPosition * 1.85;
		}
		else
		{
			x = projectedScreenPosition[0];
			y = projectedScreenPosition[1];
		}

		float xScale = m_fOriginalXSize * scale;
		float yScale = m_fOriginalYSize * scale;
		// Apply
		FrameSlot.SetPos(m_wRoot, x - 0.5 * xScale, y - 0.5 * yScale);

		// Scale
		FrameSlot.SetSize(m_wLayout, xScale, yScale);

		m_wNameText.SetExactFontSize(m_fOriginalTextSize * textScale);

		// Set objective symbol
		m_wNameText.SetText(m_pAffiliatedArea.GetPointName());

		// And make widget visible
		m_wRoot.SetVisible(true);
	}
}