// View.h : interface of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

using HTLItem = UINT;

class CTreeListView : public CWindowImpl<CTreeListView, CListViewCtrl> {
public:
	DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

		HTLItem AddChildItem(HTLItem index, PCWSTR text, int image);
	HTLItem AddItem(PCWSTR text, int image);
	bool IsExpanded(HTLItem hItem) const;
	bool CollapseItem(HTLItem hItem);
	bool ExpandItem(HTLItem hItem);
	bool SetIcon(HICON hIcon, bool expanded);
	bool SetItemText(HTLItem hItem, int subItem, PCWSTR text);

protected:
	void DoCollapseItem(HTLItem hItem);
	void DoExpandItem(HTLItem hItem);

	BEGIN_MSG_MAP(CTreeListView)
		REFLECTED_NOTIFY_CODE_HANDLER(NM_CLICK, OnClick)
		MESSAGE_HANDLER(LVM_DELETEITEM, DoDeleteItem)
		MESSAGE_HANDLER(WM_CREATE, DoCreate)
		if (!m_SuspendSetItem) {
			MESSAGE_HANDLER(LVM_SETITEMTEXT, OnSetItemText)
				MESSAGE_HANDLER(LVM_SETITEM, OnSetItem)
		}
	END_MSG_MAP()

	struct ListViewItem : LVITEMW {
		WCHAR Text[64];
		UINT Id;
		std::vector<std::wstring> SubItems;
		bool Collapsed{ false };

		ListViewItem() {
			pszText = Text;
			cchTextMax = _countof(Text);
		}
	};
	int InsertChildItems(int index);
	int InsertChildItems(int index, std::vector<HTLItem>& children);

	void SuspendSetItemText(bool suspend = true);
	bool DoSetItemText(HTLItem n, int subitem, PCWSTR text);
	HTLItem SaveItem(int index);

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInsertItem(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnSetItemText(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSetItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT DoDeleteItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT DoCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	std::unordered_map<HTLItem, std::vector<HTLItem>> m_Collapsed;
	std::unordered_map<HTLItem, ListViewItem> m_HiddenItems;
	bool m_SuspendSetItem{ false };
	bool m_Deleting{ false };
};