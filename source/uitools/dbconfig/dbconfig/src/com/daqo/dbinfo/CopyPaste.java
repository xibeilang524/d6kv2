package com.daqo.dbinfo;

import javax.swing.tree.DefaultMutableTreeNode;
import java.util.Vector;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//保存Copy的数据
public class CopyPaste {
  DefaultMutableTreeNode node;
  public Vector dataVector;
  public CopyPaste() {
  }

  public void setNode(DefaultMutableTreeNode Node) {
    this.node = Node;
    dataVector = new Vector(0);
  }
}
