package com.daqo.dbinfo.renderer;

import javax.swing.JTable;
import java.awt.event.MouseListener;
import java.awt.event.MouseEvent;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class ListTreeRowHeaderRender
    implements MouseListener {
  public JTable rowheader;
  public ListTreeRowHeaderRender(JTable rowheader) {
    this.rowheader = rowheader;
  }

  public void mouseClicked(MouseEvent e) {
  }

  public void mouseEntered(MouseEvent e) {
  }

  public void mouseExited(MouseEvent e) {
  }

  public void mousePressed(MouseEvent e) {
  }

  public void mouseReleased(MouseEvent e) {
    rowheader.updateUI();
  }
}
