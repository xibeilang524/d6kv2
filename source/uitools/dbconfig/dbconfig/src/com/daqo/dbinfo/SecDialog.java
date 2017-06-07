package com.daqo.dbinfo;

import javax.swing.*;
import java.awt.*;
import com.borland.jbcl.layout.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class SecDialog
    extends JDialog {

 private  boolean bcancelfalg;
  public SecDialog(Frame frame, String title, boolean modal) {
    super(frame,title,modal);
 //   super( new FrameDB() , "输入对话框", true);
    bcancelfalg = true;
  }

  private void jbInit() throws Exception {
  }

  public boolean getCancelF()
      {
        return bcancelfalg;
      }

  public void setCancelF(boolean bcancelf)
      {
        this.bcancelfalg = bcancelf;
      }
}
