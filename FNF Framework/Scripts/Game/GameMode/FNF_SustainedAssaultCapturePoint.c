//-----------------------------------------------------------------------------------------------
class FNF_SustainedAssaultCapturePointClass : SCR_CaptureAreaClass
{
};

//------------------------------------------------------------------------------------------------
class FNF_SustainedAssaultCapturePoint : SCR_CaptureArea
{
	[Attribute("A", uiwidget: UIWidgets.EditBox, "Name of this capture point for Players to see", category: "Capture Point")]
	protected string m_sPointName;
	
	[Attribute("10", uiwidget: UIWidgets.Slider, "Time it takes to capture this zone", params: "0 864000 1", category: "Capture Point")]
	protected float m_fTimeToCapture;
	
	[Attribute("0 0 0", UIWidgets.EditBox, "Center of the objective in local space.", category: "CaptureAndHold", params: "inf inf 0 purposeCoords spaceEntity")]
	protected vector m_vObjectiveCenter;
	
	protected Faction m_CurrentTimedFaction = null;
	protected Faction m_AttackingFaction = null;
	protected float m_CurrentOwnedPercent = 100.0;
	protected int m_CurrentOwnedPercentIncreasingRate = 0;
	
	string GetPointName()
	{
		return m_sPointName;
	}
	
	protected override void OnInit(IEntity owner)
	{
		super.OnInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		FNF_GameModeSustainedAssault m_pGameMode = FNF_GameModeSustainedAssault.Cast(GetGame().GetGameMode());
		
		RegisterWithGamemode(m_pGameMode);
	}
	
	override protected event void OnOwningFactionChanged(Faction previousFaction, Faction newFaction)
	{
		super.OnOwningFactionChanged(previousFaction, newFaction);
  		if (newFaction == m_CurrentTimedFaction && newFaction != null)
		{
			m_CurrentOwnedPercentIncreasingRate = -1;
		}
		else if (newFaction == m_AttackingFaction)
		{
			m_CurrentOwnedPercentIncreasingRate = 1;
		}
		else if (newFaction != null)
		{
			m_CurrentOwnedPercentIncreasingRate = 1;
		}
		else
		{
			m_CurrentOwnedPercentIncreasingRate = 0;
		}
	}
	
	override protected event void OnFrame(IEntity owner, float timeSlice)
	{
		super.OnFrame(owner, timeSlice);
		if (m_CurrentOwnedPercentIncreasingRate != 0)
		{
			if (m_CurrentOwnedPercent <= 100 && m_CurrentOwnedPercent >= 0) 
			{
				if (m_CurrentOwnedPercentIncreasingRate == 1)
				{
					m_CurrentOwnedPercent = m_CurrentOwnedPercent + ((100 / m_fTimeToCapture) * timeSlice)
				}
				else
				{
					m_CurrentOwnedPercent = m_CurrentOwnedPercent - ((100 / m_fTimeToCapture) * timeSlice)
				}
			}
			else if (m_CurrentOwnedPercent <= 0)
			{
				m_CurrentTimedFaction = GetOwningFaction();
				m_CurrentOwnedPercent = 0.0;
				m_CurrentOwnedPercentIncreasingRate = 0;
			}
			else if (m_CurrentOwnedPercent >= 100)
			{
				if (GetOwningFaction() != m_AttackingFaction)
				{
					m_AttackingFaction = GetOwningFaction();
					m_CurrentOwnedPercent = 0.0;
					m_CurrentOwnedPercentIncreasingRate = 1;
				}
				else
				{
					m_CurrentTimedFaction = GetOwningFaction();
					m_CurrentOwnedPercent = 100.0;
					m_CurrentOwnedPercentIncreasingRate = 0;
				}
			}
		}
	}
	
	Faction GetCurrentTimedFactionOwner()
	{
		return m_CurrentTimedFaction;
	}
	
	Faction GetAttackingFaction()
	{
		return m_AttackingFaction;
	}
	
	float GetCurrentOwnedPercent()
	{
		return m_CurrentOwnedPercent;
	}
	
	protected void RegisterWithGamemode(FNF_GameModeSustainedAssault gameMode)
	{
		gameMode.RegisterCapturePoint(this);	
	}
	
	vector GetWorldObjectiveCenter()
	{
		return CoordToParent(m_vObjectiveCenter);
	}

	bool IsCharacterInside(SCR_ChimeraCharacter character)
	{
		if (!character)
			return false;

		Faction faction = character.GetFaction();
		if (!faction)
			return false;

		int indexOf = m_mOccupants[faction].Find(character);
		return indexOf != -1;
	}
};