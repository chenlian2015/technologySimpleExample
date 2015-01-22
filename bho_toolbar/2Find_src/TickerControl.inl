  struct Ticker : public Button
  {
    Ticker() {m_pos_last=0; m_last=QueryCounter();}
    virtual void onOptionChange(CBandToolBarCtrl* ctrl)
    {
      ctrl->hideIfNeeded(m_id,m_command);
    }

    CFont font;
    virtual void change(CBandToolBarCtrl* ctrl)
    {
      Button::change(ctrl);

      TBBUTTONINFO tbi;
      tbi.cbSize = sizeof TBBUTTONINFO;
      tbi.dwMask = TBIF_SIZE | TBIF_STYLE;
      tbi.fsStyle = TBSTYLE_BUTTON | BTNS_SHOWTEXT;
      tbi.cx = m_style.m_width;
      ctrl->SetButtonInfo(WM_USER+m_id, &tbi);

      font.CreateFont(m_style.m_fontsize,0, 0, 0, FW_NORMAL, 0, 0, 0,DEFAULT_CHARSET,
                     OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                     DEFAULT_PITCH | FF_SWISS, m_style.m_font.c_str());

      CDC dc = ::GetDC(NULL);
      dc.SelectFont(font.m_hFont);
      for(int i=0;i<m_items.size();i++)
        GetTextExtentPoint(dc,m_items[i]->m_value.c_str(),m_items[i]->m_value.size(),&m_items[i]->m_size);
    }
    virtual void determine(XmlParserProxy& p)
    {
      ToolbarItem::determine(p);
      XML::determineMember<TickerItem>(m_items,_T("tickeritem"),p);
      XML::determineMember<>(m_style,_T("tickerstyle"),p);
    }
   private:
     virtual void Invalidate(CBandToolBarCtrl* ctrl)
     {
      CRect rc;
      ctrl->GetItemRect(m_id, &rc);
      rc.right = rc.left + m_style.m_width;
      rc.bottom = TB_HEIGHT;
      if(ctrl->IsWindow())
        ctrl->InvalidateRect(rc,FALSE);
     }
     virtual void onCommand(CBandToolBarCtrl* ctrl)
     {
        float now = QueryCounter();
        float delta = now - m_last;
        m_last = now;
        m_pos_last += delta*m_style.m_speed;

        while(m_pos_last>=m_items.size())
          m_pos_last -= m_items.size();

        int cur = (int)m_pos_last;
        if(Commands::Command* c = ctrl->m_toolbar->selectCommand(m_items[cur]->m_command))
          c->apply(ctrl);
     }
     virtual DWORD OnItemPrePaint(CBandToolBarCtrl* ctrl,LPNMTBCUSTOMDRAW lpTBCustomDraw)
     {
        float now = QueryCounter();
        float delta = now - m_last;
        m_last = now;
        m_pos_last += delta*m_style.m_speed;

        while(m_pos_last>=m_items.size())
          m_pos_last -= m_items.size();

        int cur = (int)m_pos_last;
        float percent = (m_pos_last - cur);

        percent *= (1+m_items[cur]->m_delay);

        if(percent>1)
          percent = 0;

        CDCHandle dc = lpTBCustomDraw->nmcd.hdc;

		    dc.SetTextColor(m_style.m_textcolor);
        dc.SetBkColor(m_style.m_background);
		    dc.SetBkMode(OPAQUE);
        dc.SelectFont(font.m_hFont);

        CBrush back(CreateSolidBrush(m_style.m_background));

        // make height from XML
        int cur_height = lpTBCustomDraw->nmcd.rc.bottom - lpTBCustomDraw->nmcd.rc.top;
        int middle = (lpTBCustomDraw->nmcd.rc.bottom + lpTBCustomDraw->nmcd.rc.top)/2;
        lpTBCustomDraw->nmcd.rc.bottom = middle + m_style.m_height/2;
        lpTBCustomDraw->nmcd.rc.top    = middle - m_style.m_height/2;

        // draw border
        CPen pen(CreatePen(0, m_style.m_border, m_style.m_bordercolor));
        CRect r = lpTBCustomDraw->nmcd.rc;
        HPEN old = dc.SelectPen(pen);
        dc.MoveTo(r.left,r.bottom);
        POINT lpPoints[] = { {r.left,r.top},{r.right,r.top},{r.right,r.bottom},{r.left,r.bottom} };
        dc.PolylineTo(lpPoints,4);
        dc.SelectPen(old);


        // correct due to border rect
        lpTBCustomDraw->nmcd.rc.left  += m_style.m_border - 1;
        lpTBCustomDraw->nmcd.rc.right -= m_style.m_border - 2;

        // clip region
        CRgn hrgn;
        hrgn.CreateRectRgnIndirect(&lpTBCustomDraw->nmcd.rc);
        SelectClipRgn(lpTBCustomDraw->nmcd.hdc,hrgn);

        lpTBCustomDraw->nmcd.rc.top   += m_style.m_border - 1;
        lpTBCustomDraw->nmcd.rc.bottom-= m_style.m_border - 2;

        // text height
        int height = m_items[cur]->m_size.cy;
        int h = lpTBCustomDraw->nmcd.rc.bottom - lpTBCustomDraw->nmcd.rc.top;

        // up
        CRect rc2 = lpTBCustomDraw->nmcd.rc;
        rc2.bottom = lpTBCustomDraw->nmcd.rc.top + (h-height)/2 + 1;
        FillRect(lpTBCustomDraw->nmcd.hdc, &rc2, back);
        // down
        rc2.top = lpTBCustomDraw->nmcd.rc.bottom - (h-height)/2 - 1;
        rc2.bottom = lpTBCustomDraw->nmcd.rc.bottom;
        FillRect(lpTBCustomDraw->nmcd.hdc, &rc2, back);

        lpTBCustomDraw->nmcd.rc.top    += (h-height)/2;
        lpTBCustomDraw->nmcd.rc.bottom -= (h-height)/2;

        CRect rc = lpTBCustomDraw->nmcd.rc;

        // left    :  rc.left - m_items[cur]->m_size.cx   ......... rc.right
        // percent :                0                                   1
        int width = m_items[cur]->m_size.cx;
        int w = (rc.right-(rc.left - width));
        lpTBCustomDraw->nmcd.rc.left += w*(1-percent) - width;

        // left rect before text
        rc.right = lpTBCustomDraw->nmcd.rc.left;
        FillRect(lpTBCustomDraw->nmcd.hdc, &rc, back);

        // after text
        rc.right = lpTBCustomDraw->nmcd.rc.right;
        rc.left = lpTBCustomDraw->nmcd.rc.left+width;
        FillRect(lpTBCustomDraw->nmcd.hdc, &rc, back);

        lpTBCustomDraw->nmcd.rc.right = rc.left;
        DrawText(lpTBCustomDraw->nmcd.hdc,m_items[cur]->m_value.c_str(), -1, &lpTBCustomDraw->nmcd.rc, DT_VCENTER | DT_SINGLELINE);

        DeleteObject(hrgn);

        //CRgn hrgn1;
        //CRect rrr(0,0,1,1);
        //hrgn1.CreateRectRgnIndirect(&rrr);
        //SelectClipRgn(lpTBCustomDraw->nmcd.hdc,hrgn1);
        SelectClipRgn(lpTBCustomDraw->nmcd.hdc,0);
        //DeleteObject(hrgn1);
        return CDRF_SKIPDEFAULT;
       //return CDRF_DODEFAULT | TBCDRF_NOEDGES;
     }
     float m_last;
     float m_pos_last;
     struct TickerStyle : public XmlObject
     {
       TickerStyle() : m_fontsize(10) {}

        int     m_border;
        int     m_bordercolor;
        int     m_background;
        zstring m_font;
        int     m_fontsize;
        int     m_textcolor;
        int     m_width;
        int     m_height;
        float   m_speed;
		    virtual void determine(XmlParserProxy& p)
		    {
			    XML::determineMember<>(m_font,_T("font"),p);
			    XML::determineMember<>(m_fontsize,_T("fontsize"),p);
			    XML::determineMember<>(m_textcolor,_T("textcolor"),p);
			    XML::determineMember<>(m_border,_T("border"),p);
			    XML::determineMember<>(m_bordercolor,_T("bordercolor"),p);
			    XML::determineMember<>(m_background,_T("background"),p);
			    XML::determineMember<>(m_width,_T("width"),p);
			    XML::determineMember<>(m_height,_T("height"),p);
			    XML::determineMember<>(m_speed,_T("speed"),p);
		    }
	   }m_style;
     struct TickerItem : public XmlObject
     {
        TickerItem() : m_delay(0){}
		    zstring m_command;
		    zstring m_value;
        float   m_delay;
        SIZE    m_size; // width of string in pixels (precalculated)
		    virtual void determine(XmlParserProxy& p)
		    {
			    XML::determineMember<>(m_delay,_T("delay"),p);
			    XML::determineMember<>(m_command,_T("command"),p);
			    m_value = p.load();
		    }
	   };
	   XML::vectorDel<TickerItem*> m_items;
  };
