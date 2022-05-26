//------------------------------------------------------------------------------------------------
class FNF_GameModeSustainedAssaultClass : SCR_BaseGameModeClass
{
};

//------------------------------------------------------------------------------------------------
class FNF_GameModeSustainedAssault : SCR_BaseGameMode
{
	
	protected ref array<FNF_SustainedAssaultCapturePoint> m_aAreas = {};
	
	void RegisterCapturePoint(FNF_SustainedAssaultCapturePoint point)
	{
		m_aAreas.Insert(point);	
	}
	
	void UnRegisterCapturePoint(FNF_SustainedAssaultCapturePoint point)
	{
		int index = m_aAreas.Find(point);
		if (index != -1)
		{
			m_aAreas.Remove(index);
		}
	}
	
	int GetAreas(notnull array<FNF_SustainedAssaultCapturePoint> outAreas)
	{
		int count = 0;
		foreach (FNF_SustainedAssaultCapturePoint area : m_aAreas)
		{
			outAreas.Insert(area);
			++count;
		}
		return count;
	}
}