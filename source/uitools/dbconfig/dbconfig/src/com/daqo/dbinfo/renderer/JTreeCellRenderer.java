package com.daqo.dbinfo.renderer;

import javax.swing.JTree;
import javax.swing.tree.DefaultTreeCellRenderer;
import java.awt.Component;
import javax.swing.tree.DefaultMutableTreeNode;
import java.util.Vector;


import com.daqo.dbinfo.ListTree;


import javax.swing.ImageIcon;
import java.awt.Color;

import com.daqo.dbinfo.ConstantDef;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class JTreeCellRenderer
    extends DefaultTreeCellRenderer {

  public JTreeCellRenderer() {
    init();

  }

  public void init() {
      setClosedIcon(new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/parentnode.png")));
      setLeafIcon(new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/node.png")));
      setOpenIcon(new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/parentnode.png")));
      setTextSelectionColor(Color.blue   );
      setOpaque(true);
      setBackgroundNonSelectionColor(Color.white);
  }


  public Component getTreeCellRendererComponent(
      JTree tree,
      Object value,
      boolean sel,
      boolean expanded,
      boolean leaf,
      int row,
      boolean hasFocus) {

    super.getTreeCellRendererComponent(
        tree, value, sel,
        expanded, leaf, row,
        hasFocus);
    if (leaf) {
      setToolTipText(value.toString());
      ListTree listtree = (ListTree)tree;
      Vector vNode = listtree.vOpenLeafNode ;
      int level;
      //当前表的颜色不同
      for(int i = 0; i < vNode.size() ; i++)
        {
        DefaultMutableTreeNode node = (DefaultMutableTreeNode)vNode.elementAt(i) ;
        DefaultMutableTreeNode nodevalue = (DefaultMutableTreeNode)value;
        if(node.equals(nodevalue) )
          {
            setIcon(new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/selnode.png"))) ;
            level = node.getLevel() ;
            String nodetb = "";
            if(level  <= 2)
              {
                nodetb = node.toString() ;
              }
              else if(level == 3)
                {
                  nodetb = node.getParent().toString() + "/" + node.toString() ;
                }
              else
                {
                  nodetb = node.getParent().getParent().toString() + "/"
                   + node.getParent().toString() + "/" +
                           node.toString() ;
                }

            if(nodetb.equalsIgnoreCase(listtree.m_curtablename  ) )
              {
                setIcon(new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource("icon/curselnode.png"))) ;
              }
          }
        }
    }

    return this;
  }

  public String getNodePath(Object value) {
    String path = "";
    if (value == null) {
      return path;
    }
    DefaultMutableTreeNode node = (DefaultMutableTreeNode) value;
    while (!node.isRoot()) {
      if (path.equals("")) {
        path = node.getParent().toString();
      }
      else {
        path = node.getParent().toString() + "\\" + path;
      }
      node = (DefaultMutableTreeNode) node.getParent();
    }
    return path;
  }
}
