class FNF_CorePlayZoneClass : SCR_CaptureAreaClass
{
};

class FNF_CorePlayZone : SCR_CaptureArea
{
	[Attribute("10", uiwidget: UIWidgets.Slider, "Time it takes for a player to die while exiting play zone", params: "0 864000 1", category: "Play Zone")]
	protected float m_TimeToDeath;
	
	protected float m_Timer;
	
	protected override event void OnCharacterExit(Faction faction, SCR_ChimeraCharacter character)
	{
		super.OnCharacterExit(faction,character);
		character.GetInfoDisplay();
	}
	protected override void OnInit(IEntity owner)
	{
		super.OnInit(owner);
		m_Timer = m_TimeToDeath;
	}

};

class FNF_PlayerOutOfBounds
{
	protected float m_TimeLeftToDeath;
	protected SCR_ChimeraCharacter m_Character;
	
	void FNF_PlayerOutOfBounds (float timeToDeath, SCR_ChimeraCharacter character)
	{
		m_TimeLeftToDeath = timeToDeath;
		m_Character = character;
	}
	
	void SetTimeLeftToDeath (float timeToDeath)
	{
		m_TimeLeftToDeath = timeToDeath;
	}
	
	float GetTimeLeftToDeath ()
	{
		return m_TimeLeftToDeath;
	}
}