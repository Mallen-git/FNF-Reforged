class FNF_CoreNotificationDisplay : SCR_InfoDisplayExtended
{

	//! Game mode instance.
	protected FNF_GameModeSustainedAssault m_pGameMode;
	
	protected ref array<ref FNF_NotificationObject> m_NotificationQ = {};
	
	protected FNF_NotificationObject m_CurrentNotification;
	protected float m_Timer;
	
	protected TextWidget m_TitleText;
	protected RichTextWidget m_MessageText;
	protected OverlayWidget m_Overlay;
	protected ImageWidget m_Background;

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
		
		FNF_CoreNotificationManagerComponent notifGang = FNF_CoreNotificationManagerComponent.Cast(m_pGameMode.FindComponent(FNF_CoreNotificationManagerComponent));
		
		notifGang.RegisterNotificationDisplay(this);

		return true;
	}

	//------------------------------------------------------------------------------------------------
	/*
		Creates HUD elements.
	*/
	override void DisplayStartDraw(IEntity owner)
	{
		m_TitleText = TextWidget.Cast(m_wRoot.FindAnyWidget("Title_Text"));
		m_MessageText = RichTextWidget.Cast(m_wRoot.FindAnyWidget("Message_Text"));
		m_Overlay = OverlayWidget.Cast(m_wRoot.FindAnyWidget("Overlay"));
		m_Background = ImageWidget.Cast(m_wRoot.FindAnyWidget("Background"));
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Updates and manages individual HUD elements.
	*/
	override void DisplayUpdate(IEntity owner, float timeSlice)
	{
		if (m_NotificationQ.Count() != 0)
		{
			if (m_CurrentNotification == m_NotificationQ[0])
			{
				m_Timer = m_Timer - timeSlice;
				float height = 90 + (m_MessageText.GetNumLines() * 24);
				vector size = m_Background.GetSize();
				if (size != Vector(400, height, 0))
				{
					m_Background.SetSize(400,height);	
				}
			}
			else
			{
				m_Overlay.SetVisible(true);
				m_CurrentNotification = m_NotificationQ[0];
				m_TitleText.SetText(m_CurrentNotification.GetTitleText());
				m_MessageText.SetText(m_CurrentNotification.GetMessageText());
				m_Timer = m_CurrentNotification.GetTimeToShow();
				float height = 90 + (m_MessageText.GetNumLines() * 24);
				m_Background.SetSize(400,height);
			}
			if (m_Timer <= 0)
			{
				m_NotificationQ.Remove(0);
				m_Overlay.SetVisible(false);
			}
		}
		else
		{
			m_Overlay.SetVisible(false);
		}
	}
	
	void AddNotification(FNF_NotificationObject notification, bool showInstantly)
	{
		if (showInstantly)
		{
			m_NotificationQ.InsertAt(notification,0);
		}
		else
		{
			m_NotificationQ.Insert(notification);
		}
	}
}

class FNF_NotificationObject
{
	protected string m_TitleText;
	protected string m_MessageText;
	protected float m_TimeToShow;
	
	void FNF_NotificationObject (string title, string message, float timeToShow)
	{
		m_TitleText = title;
		m_MessageText = message;
		m_TimeToShow = timeToShow;
	}
	
	string GetTitleText()
	{
		return m_TitleText;
	}
	
	string GetMessageText()
	{
		return m_MessageText;
	}
	
	float GetTimeToShow()
	{
		return m_TimeToShow;
	}
}