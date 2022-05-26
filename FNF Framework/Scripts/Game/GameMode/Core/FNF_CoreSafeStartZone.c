class FNF_CoreSafeStartZoneClass : SCR_CaptureAreaClass
{
};

class FNF_CoreSafeStartZone : SCR_CaptureArea
{
	[Attribute("10", uiwidget: UIWidgets.Slider, "Determines how long it takes for a player to be teleported back when exiting safe start", params: "0 864000 1", category: "Safe Start")]
	protected float m_TimeToDeath;
	
	[Attribute("Red", UIWidgets.EditBox, "Determines which faction this safe start is for", category: "Safe Start")]
	private string m_Faction;
};