#pragma once

class UI;
class PannelUI;

class UIMgr
	:public Singleton<UIMgr>
{
	friend class Singleton;
	UIMgr();
	~UIMgr();
	static const std::function<bool(const UI*, const UI*)> cmpZDepth;
private:
	vector<shared_ptr<PannelUI>> m_vecUI[etoi(SCENE_TYPE::END)];
	SCENE_TYPE m_ePrevUIScene = SCENE_TYPE::INTRO;
	SCENE_TYPE m_eCurUIScene = SCENE_TYPE::INTRO;
	std::set<PannelUI*, decltype(cmpZDepth)> m_setUI{cmpZDepth};

private:
	int m_iSelectedIndex = 0;
	shared_ptr<PannelUI> m_pTargetUI;

public:
	void Init();
	void Start();
	void Update();
	void Render();
	void AddUI(shared_ptr<PannelUI> pUI) { m_vecUI[etoi(m_eCurUIScene)].emplace_back(std::move(pUI)); }
	vector<shared_ptr<PannelUI>>& GetUIVec() { return m_vecUI[etoi(m_eCurUIScene)]; }

	void SaveForPractice(string_view _strPracticeName);
	void LoadForPractice(string_view _strPracticeName);
	
	void InputUpdate();

	void Reset()noexcept
	{
		for (auto& vec : m_vecUI)
		{
			vec.clear();
		}
	}
	void UnLoadUIScene()noexcept 
	{
		std::swap(m_eCurUIScene, m_ePrevUIScene);
	}
	void ChangeUIScene(SCENE_TYPE eUIScene)noexcept
	{
		m_ePrevUIScene = m_eCurUIScene;
		m_eCurUIScene = eUIScene;
	}

public:
	int GetSelectIndex() const { return m_iSelectedIndex; };
	void SetSelectIndex(int index);
};
