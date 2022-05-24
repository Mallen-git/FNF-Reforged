//------------------------------------------------------------------------------------------------
/*!
	This InfoDisplay allows drawing of individual HUD objective elements for individual
	Capture & Hold areas by projecting their position from world to screen.
*/
class FNF_SustainedAssaultIconDisplay : SCR_InfoDisplayExtended
{
	[Attribute("", params: "layout")]
	protected ResourceName m_rObjectiveHUDLayout;

	//! Individual element wrappers
	protected ref array<ref FNF_SustainedAssaultIconDisplayObject> m_aObjectiveElements = {};

	//! Game mode instance.
	protected FNF_GameModeSustainedAssault m_pGameMode;

	//------------------------------------------------------------------------------------------------
	/*!
		Checks whether prerequisites for this InfoDisplay are met.
	*/
	override bool DisplayStartDrawInit(IEntity owner)
	{
		// No ui can be drawn without necessary items
		m_pGameMode = FNF_GameModeSustainedAssault.Cast(GetGame().GetGameMode());
		if (!m_pGameMode)
			return false;

		return true;
	}

	//------------------------------------------------------------------------------------------------
	/*
		Creates HUD elements for individual areas.
	*/
	override void DisplayStartDraw(IEntity owner)
	{
		// Create objective icons
		array<FNF_SustainedAssaultCapturePoint> areas = {};
		int cnt = m_pGameMode.GetAreas(areas);
		CreateObjectiveDisplays(areas);
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Destroys HUD elements of individual areas.
	*/
	override void DisplayStopDraw(IEntity owner)
	{
		// Clear ALL hud objects
		ClearObjectiveDisplays(false);
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Updates and manages individual HUD elements.
	*/
	override void DisplayUpdate(IEntity owner, float timeSlice)
	{
		// area markers
		array<FNF_SustainedAssaultCapturePoint> areas = {};
		int count = m_pGameMode.GetAreas(areas);
		World world = GetGame().GetWorld();
		IEntity playerEntity = GetGame().GetPlayerController().GetControlledEntity();

		// Filter out entries which do not link to area anymore
		ClearObjectiveDisplays(true);
		// Redraw all elements
		if (areas.Count() != m_aObjectiveElements.Count())
		{
			ClearObjectiveDisplays(false);
			CreateObjectiveDisplays(areas);
		}

		for (int i = 0; i < count; i++)
			m_aObjectiveElements[i].UpdateDynamic(playerEntity, timeSlice);
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Creates and fills the objective collection using provided areas.
	*/
	protected void CreateObjectiveDisplays(array<FNF_SustainedAssaultCapturePoint> areas)
	{
		for (int i = 0, count = areas.Count(); i < count; i++)
		{
			Widget objective = GetRootWidget().GetWorkspace().CreateWidgets(m_rObjectiveHUDLayout, GetRootWidget());
			if (!objective)
				continue;

			FNF_SustainedAssaultIconDisplayObject displayObject = new FNF_SustainedAssaultIconDisplayObject(objective, areas[i]);
			m_aObjectiveElements.Insert(displayObject);
		}
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Removes objective displays from the collection.
		\param emptyOnly If true, empty (not linking to area) objects are removed. If false, all objects are flushed immediately.
	*/
	protected void ClearObjectiveDisplays(bool emptyOnly)
	{
		// Clear previous elements
		for (int i = m_aObjectiveElements.Count() - 1; i >= 0; i--)
		{
			if (emptyOnly && m_aObjectiveElements[i].GetArea())
				continue;

			Widget w = m_aObjectiveElements[i].GetRootWidget();
			if (w)
			{
				w.RemoveFromHierarchy();
				delete w;
			}

			m_aObjectiveElements.Remove(i);
		}
	}
}