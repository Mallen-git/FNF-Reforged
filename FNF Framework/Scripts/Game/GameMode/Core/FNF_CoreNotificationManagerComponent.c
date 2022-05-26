[ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Component for managing notifications for players")]
class FNF_CoreNotificationManagerComponentClass : ScriptComponentClass
{}

class FNF_CoreNotificationManagerComponent : ScriptComponent
{
	
	FNF_CoreNotificationDisplay m_Display;
	
	protected ref array<ref FNF_NotificationObject> m_NotificationQ = {};
	
	void RegisterNotificationDisplay(FNF_CoreNotificationDisplay display)
	{
		m_Display = display;
	}
	
	bool AddNotification(FNF_NotificationObject notification, bool showInstantly)
	{
		if (m_Display)
		{
			m_Display.AddNotification(notification, showInstantly);
			return true;
		}
		else
		{
			return false;
		}
	}
}