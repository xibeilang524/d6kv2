package com.daqo.dbinfo;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.tree.*;
import java.text.ParseException;
import java.text.NumberFormat;
import java.text.DecimalFormat;
import java.awt.Cursor;
import java.text.SimpleDateFormat;

import com.daqo.dbinfo.table.*;
import com.daqo.dbinfo.renderer.*;
import java.net.UnknownHostException;
import java.net.InetAddress;


/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//保存参数的Vector成员变量，一般包含两层：内层VectorIn保存表的每分区的设置，外层VectorOut包含每个表的设置
public class ListTree
    extends JTree {

  JTabbedPane jtab;
  Dlllink pdll = Dlllink.getInstance();
  static DefaultMutableTreeNode rootnode = new DefaultMutableTreeNode("数据库应用");
  PDB_FieldTable pdb_fieldtable; //pdb_field的域属性
  Vector vListTable = new Vector(0); //save 每个表的jtable

  Vector vTabAreaData = new Vector(0); //保存表的tab分区的columnname和data数据,格式为tab1的col,data,tab2的col,data,...。
  Vector vTableTabnoCount = new Vector(0); //每个表的分区数
  public Vector vOpenLeafNode = new Vector(0); //每个打开的叶子节点

  Vector vTabnoJTab = new Vector(0); //有分区的表的JTabbedPane
  Vector vColumnClass = new Vector(0); //每列的类型,当表为空时，MyTableModel的getColumnClass函数需要
  Vector vLongValues = new Vector(0); //存储每列最长的值
  ServiceTypeTable[] serviceTypeTable = null;
  AreaTable[] AreaTable =null;
  StationTable[] StationTable =null;
  VoltageTable[] VoltageTable =null;
  Separate[] Separate=null;     //南网测试2012
  ScdTypeTable[] ScdTypeTable= null;
  DevTable[] DevTable=null;
  Vector vKey = new Vector(0); //存放主键的列号
  public Vector vTitle =new Vector(0);  //hongxiang 存放每个打开表的树路径
  public String flwhere;
  public boolean vSelectFlag = false;
  ImageIcon imagetab = null;
  int fontsize, rowheight;
  DefaultMutableTreeNode m_node;
  String m_db, m_tablename, m_where, m_orderfn;
  public String m_curtablename;
  int m_tabletabcount;
  Vector m_vTabnoName = new Vector(0); //存放分区名
  String m_realtable;
  TableSetInfo tablesetinfo;
  String m_slistfield = ""; //列表域名
  Vector m_vlistfield = new Vector(0); //

  ManageModel m_managemodel;
  private final SimpleDateFormat dateFormat =
      new SimpleDateFormat("yyHHmmssSSS");
  char letter[] = {
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e',
      'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
      'u', 'v', 'w', 'x', 'y', 'z'};
  char dev[] = {
      ' ', 'K', 'D', 'B', 'M', 'B', 'F', 'C', 'R', 'Q', 'U', 'I', 'L', 'D', 'D',
      'M', 'F', 'D', 'X', 'D', 'H', 'P', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
      ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
      ' ', ' ', ' ', ' ', ' ', 'O', 'S'};

  public ListTree(JTabbedPane jTab) {

    super(new DefaultTreeModel(rootnode));
    javax.swing.plaf.TreeUI tuiw = new com.sun.java.swing.plaf.windows.
        WindowsTreeUI();
    this.setUI(tuiw);
    this.setBackground(Color.white);
    this.setOpaque(true);
    this.jtab = jTab;
    getSelectionModel().setSelectionMode(TreeSelectionModel.
                                         SINGLE_TREE_SELECTION); //设置树的选择模式
    JTreeCellRenderer jTreeCelRender = new JTreeCellRenderer();
    setCellRenderer(jTreeCelRender);
    fontsize = 14;
    rowheight = 20;
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    if (screenSize.width == 800) {
      fontsize = 12;
      rowheight = 20;
    }
    else if (screenSize.width == 1024) {
      fontsize = 14;
      rowheight = 20;
    }
    else if (screenSize.width == 1152) {
      fontsize = 14;
      rowheight = 22;
    }
    else if (screenSize.width == 1280) {
      fontsize = 16;
      rowheight = 24;
    }
    else if (screenSize.width >= 1600) {
      fontsize = 18;
      rowheight = 24;
    }

    try {
      super.setFont(new java.awt.Font(ConstantDef.FONTNAME, 0, fontsize));
      imagetab = new ImageIcon(com.daqo.dbinfo.ListTree.class.getResource(
          "icon/tableopen.png"));
    }
    catch (NullPointerException npe) {}
  }

  public void addRootNodes() {
    byte[][] buf;
    int[] fieldcount = new int[1];
    buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.SERVICE_TYPE,
                             "服务类型,服务描述,参数库名", "", "", fieldcount, false);
    int len = buf.length;
    if (len > 0) {
      serviceTypeTable = new ServiceTypeTable[len / fieldcount[0]];
      for (int i = 0; i < len; i++) {
        if (i % fieldcount[0] == 0) {
          serviceTypeTable[i / fieldcount[0]] = new ServiceTypeTable();
          serviceTypeTable[i / fieldcount[0]].serviceType = pdll.substr(buf[i]); //服务类型
          serviceTypeTable[i /
              fieldcount[0]].serviceDescrible = pdll.substr(buf[i + 1]); //服务描述
          serviceTypeTable[i /
              fieldcount[0]].ParaTablename = pdll.substr(buf[i + 2]); //参数库名
          if (!serviceTypeTable[i /
              fieldcount[0]].serviceType.equalsIgnoreCase(ConstantDef.SYSTEM_DM)) {
            DefaultMutableTreeNode nodetype = new DefaultMutableTreeNode(
                serviceTypeTable[i / fieldcount[0]].serviceDescrible);
            rootnode.add(nodetype);
            serviceTypeTable[i / fieldcount[0]].isVisible = true;
          }
        }
      } //for
    } //if(len >0)
  }

  public void addSysRootNodes() {
    if (serviceTypeTable == null) {
      return;
    }
    if (ConstantDef.getUserInstance().isManageGroup()) {
      for (int i = 0; i < serviceTypeTable.length; i++) {
        if (serviceTypeTable[i].isVisible == false) {
          DefaultMutableTreeNode nodetype = new DefaultMutableTreeNode(
              serviceTypeTable[i].serviceDescrible);
          rootnode.add(nodetype);
          serviceTypeTable[i].isVisible = true;
          break;
        }
      }
    }
  }

  public void addTableNodes(DefaultMutableTreeNode node, String nodestr) {
    byte[][] buf;
    int[] fieldcount = new int[1];
    String wh = "服务描述 = '" + nodestr + "'";
    buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.SERVICE_TYPE,
                             "服务类型", wh, "", fieldcount, false);
    int len = buf.length;
    if (len > 0) {
      String servicetype = pdll.substr(buf[0]);
      wh = ConstantDef.APPTYPE + " = '" + servicetype + "'";
      if(PreProcessor.ALIAS)
        buf = pdll.queryAnyTable(ConstantDef.MODELDB,
                               ConstantDef.PARA_TABLE_DESCRIBLE,
                               ConstantDef.PARA_TABLE_DESCRIBLEKEY_ALIAS, wh,
                               "showorder",
                               fieldcount, false);

      else
         buf = pdll.queryAnyTable(ConstantDef.MODELDB,
                               ConstantDef.PARA_TABLE_DESCRIBLE,
                               ConstantDef.PARA_TABLE_DESCRIBLEKEY, wh,
                               "showorder",
                               fieldcount, false);
       len = buf.length;
       if (len > 0) {
         if (PreProcessor.ALIAS) {
           for (int i = 0; i < len; i += 2) {
             String table = pdll.substr(buf[i]).trim();
             String alias = pdll.substr(buf[i + 1]).trim();

             if (table == null) {
               continue;
             }
             //这里加入在pdb_table查询到的表名。
             DefaultMutableTreeNode nodetable;
             if (alias != null && alias.length() > 0)
               nodetable = new DefaultMutableTreeNode(alias);
             else
               nodetable = new DefaultMutableTreeNode(table);
             node.add(nodetable);
           }
         }
         else {
           for (int i = 0; i < len; i++) {
             String table = pdll.substr(buf[i]).trim();
             if (table == null) {
               continue;
             }
             //对调度人员的维护权限进行限制
             /* if (ConstantDef.getUserInstance().getGroup() ==
                  ConstantDef.USERGROUP_DD) {
                if (table.equalsIgnoreCase("SCD开关参数表") ||
                    table.equalsIgnoreCase("SCD主变压器参数表") ||
                    table.equalsIgnoreCase("SCD配电变压器参数表")) {

                }
                else {
                  continue;
                }
              }*/
             //这里加入在pdb_table查询到的表名。
             DefaultMutableTreeNode nodetable;
             nodetable = new DefaultMutableTreeNode(table);
             node.add(nodetable);
           }
         }
       }
     }
  }
  //-----CIM模型专用-----by hx  添加区域节点
  public void addAreaNodes(DefaultMutableTreeNode node, String nodestr)
  {
    byte[][] buf;
    int[] fieldcount = new int[1];
    buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.Area_TABLE,
                             "序号,描述", "", "序号", fieldcount, false);
    int len = buf.length;
    if (len > 0) {
      AreaTable = new AreaTable[len / fieldcount[0]];
      for (int i = 0; i < len; i++) {
        if (i % fieldcount[0] == 0) {
          AreaTable[i / fieldcount[0]] = new AreaTable();
          AreaTable[i / fieldcount[0]].AreaID = Integer.parseInt(pdll.substr(buf[i])); //服务类型
          AreaTable[i /
              fieldcount[0]].AreaDescrible = pdll.substr(buf[i + 1]); //服务描述
          DefaultMutableTreeNode nodetype = new DefaultMutableTreeNode(
              AreaTable[i / fieldcount[0]].AreaDescrible);
          node.add(nodetype);
        } //for
      } //if(len >0)

    }
  }
  //-----CIM模型专用-----by hx  添加站节点
  public void addStationNodes(DefaultMutableTreeNode node, String nodestr)
{
    byte[][] buf;
    int[] fieldcount = new int[1];
    int AreaID=0;
    for (int i = 0; i < AreaTable.length; i++) {
      if (AreaTable[i].AreaDescrible.equalsIgnoreCase(nodestr) ) {
         AreaID=AreaTable[i].AreaID;
         break;
      }

    }
    String wh = ConstantDef.AreaID + "=" + AreaID;
    buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.Station_TABLE,
                             "代码,描述,区域序号", "", "代码", fieldcount, false);
    int len = buf.length;
    if (len > 0) {
      StationTable = new StationTable[len / fieldcount[0]];
      for (int i = 0; i < len; i++) {
        if (i % fieldcount[0] == 0) {
          StationTable[i / fieldcount[0]] = new StationTable();
          StationTable[i /
              fieldcount[0]].StationName = pdll.substr(buf[i]); //站代码
          StationTable[i /
              fieldcount[0]].StationDescrible = pdll.substr(buf[i+1]); //站描述
          StationTable[i /
              fieldcount[0]].StationAreaID = Integer.parseInt(pdll.substr(buf[i+2])); //区域序号
          DefaultMutableTreeNode nodetype = new DefaultMutableTreeNode(
              StationTable[i / fieldcount[0]].StationDescrible);
          if(AreaID==StationTable[i /fieldcount[0]].StationAreaID){
            node.add(nodetype);
          }
        } //for
      } //if(len >0)

    }
}
  //-----CIM模型专用-----by hx  添加电压等级节点
  public void addVoltageNodes(DefaultMutableTreeNode node, String nodestr)
{
    byte[][] buf;
    int[] fieldcount = new int[1];

    buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.Voltage_TABLE,
                             "名称,电压等级", "", "电压等级", fieldcount, false);
    int len = buf.length;
    if (len > 0) {
      VoltageTable = new VoltageTable[len / fieldcount[0]];
      for (int i = 0; i < len; i++) {
        if (i % fieldcount[0] == 0) {
          VoltageTable[i / fieldcount[0]] = new VoltageTable();
          VoltageTable[i /
              fieldcount[0]].VoltageName = pdll.substr(buf[i]); //电压等级描述
          VoltageTable[i /
              fieldcount[0]].VoltageID = Integer.parseInt(pdll.substr(buf[i + 1])); //电压等级
          DefaultMutableTreeNode nodetype = new DefaultMutableTreeNode(
              VoltageTable[i / fieldcount[0]].VoltageName);
            node.add(nodetype);
        } //for
      } //if(len >0)

    }
}
  //-----CIM模型专用-----by hx  添加间隔节点
public void addSeparateNodes(DefaultMutableTreeNode node, String wh)
{
  byte[][] buf;
  int[] fieldcount = new int[1];

  buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.Separate_TABLE,
                           "描述,序号", wh, "序号", fieldcount, false);
  int len = buf.length;
  if (len > 0) {
    Separate = new Separate[len / fieldcount[0]];
    for (int i = 0; i < len; i++) {
      if (i % fieldcount[0] == 0) {
        Separate[i / fieldcount[0]] = new Separate();
        Separate[i /
            fieldcount[0]].SeparateDesc = pdll.substr(buf[i]); //电压等级描述
        Separate[i /
            fieldcount[0]].SeparateID = Integer.parseInt(pdll.substr(buf[i + 1])); //电压等级
        DefaultMutableTreeNode nodetype = new DefaultMutableTreeNode(
            Separate[i / fieldcount[0]].SeparateDesc);
          node.add(nodetype);
      } //for
    } //if(len >0)

  }
}

  //-----CIM模型专用-----by hx  添加SCD设备类型
public void addScdTypeNodes(DefaultMutableTreeNode node, String nodestr)
{
  byte[][] buf;
  int[] fieldcount = new int[1];

  buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.ScdType_TABLE,
                           "描述,类型", "", "类型", fieldcount, false);
  int len = buf.length;
  if (len > 0) {
    ScdTypeTable = new ScdTypeTable[len / fieldcount[0]];
    for (int i = 0; i < len; i++) {
      if (i % fieldcount[0] == 0) {
        ScdTypeTable[i / fieldcount[0]] = new ScdTypeTable();
        ScdTypeTable[i /
            fieldcount[0]].ScdTypeName = pdll.substr(buf[i]); //电压等级描述
        ScdTypeTable[i /
            fieldcount[0]].ScdTypeID = Integer.parseInt(pdll.substr(buf[i + 1])); //电压等级
        DefaultMutableTreeNode nodetype = new DefaultMutableTreeNode(
            ScdTypeTable[i / fieldcount[0]].ScdTypeName);
          node.add(nodetype);
      } //for
    } //if(len >0)

  }
}
//-----CIM模型专用-----by hx  添加设备节点
public void addDevNodes(DefaultMutableTreeNode node, String wh) {
  byte[][] buf;
  int[] fieldcount = new int[1];

  buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.Dev_TABLE,
                           "代码,描述", wh, "代码", fieldcount, false);
  int len = buf.length;
  if (len > 0) {
    for (int i = 0; i < len; i++) {
      if (i % fieldcount[0] == 0) {
        DefaultMutableTreeNode nodetype = new DefaultMutableTreeNode(
          pdll.substr(buf[i + 1]));
        node.add(nodetype);
      } //for
    } //if(len >0)

  }
  if (DevTable == null) {
    buf = pdll.queryAnyTable(ConstantDef.MODELDB, ConstantDef.Dev_TABLE,
                             "代码,描述", "", "代码", fieldcount, false);
    int len1 = buf.length;
    if (len1 > 0) {
      DevTable = new DevTable[len1 / fieldcount[0]];
      for (int i = 0; i < len1; i++) {
        if (i % fieldcount[0] == 0) {
          DevTable[i / fieldcount[0]] = new DevTable();
          DevTable[i /
              fieldcount[0]].DevName = pdll.substr(buf[i]); //设备代码
          DevTable[i /
              fieldcount[0]].DevDescrible = pdll.substr(buf[i + 1]); //设备描述
        } //for
      } //if(len >0)

    }
  }
}

  public void addBranchNodes(DefaultMutableTreeNode nodepara, String nodestr,
                             int level) {
    m_node = nodepara;
    m_db = "";
    m_tablename = "";
    m_where = "";
    m_orderfn = "";
    m_tabletabcount = 0;
    m_vTabnoName.removeAllElements();
    m_realtable = "";
    m_slistfield = "";
    if (level == 0) { //根节点
      addRootNodes();
      addSysRootNodes();
    }
    else if (level == 1) { //应用类型
      //CIM模型专用-----hx
      if(getModelDBType(m_node.toString()).trim().equalsIgnoreCase(ConstantDef.DEV)){
        addAreaNodes(m_node,nodestr);
      }
      //CIM模型专用-----hx
      else
      {
        getModelDB(nodestr);
        addTableNodes(m_node, nodestr);
      }

    }
    else if (level == 2) { //表名
      if (getModelDBType(m_node.getParent().toString()).trim().equalsIgnoreCase(ConstantDef.
          DEV)) {
        addStationNodes(m_node, nodestr);
      }
      else
      {
        m_db = getModelDB(m_node.getParent().toString()).trim();
        String wh = ConstantDef.TABLENAME + "='" + nodestr + "'";
        m_tablename = nodestr.trim();
        m_where = "";

        ReadPDBTable readpdbtable = new ReadPDBTable();
        int recs = readpdbtable.getPDBPropwhere(wh);
        if (recs > 0) { //有本表的设置
          m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
          m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
          m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
          m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
          m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
          m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
          if (!vSelectFlag){
          if (!readpdbtable.getAField(ConstantDef.SORTINDEXID).equalsIgnoreCase(
              "0")) { //有索引标志
            String searedtable = readpdbtable.getAField(ConstantDef.
                SEARCHEDTABLEID); //被检索表名

            if (searedtable.length() > 0) {
              String whnext = ConstantDef.TABLENAME + "='" + searedtable +
                  "' and " + ConstantDef.SORTINDEX + ">0";
              //得到第二级分类索引
              ReadPDBTable readpdbtable2 = new ReadPDBTable();
              int recs2 = readpdbtable2.getPDBPropwhere(whnext);
              if (recs2 > 0) { //先显示第二级分类索引
                boolean ret = pdll.addIndexNodes(m_node, m_db,
                                                 readpdbtable2.getAField(
                    ConstantDef.SEARCHEDTABLEID),
                                                 readpdbtable2.getAField(
                    ConstantDef.SHOWFIELDID), "",
                                                 readpdbtable2.getAField(
                    ConstantDef.SHOWFIELDID), true);
                if (!ret) { //第二级索引增加不上，忽略索引
                  m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
                  m_tabletabcount = Integer.parseInt(
                      readpdbtable.getAField(ConstantDef.TABCOUNTID));
                  m_realtable = nodestr;
                  listTableInfo();
                }
              }
              else {
                boolean ret = pdll.addIndexNodes(m_node, m_db,
                                                 readpdbtable.getAField(
                    ConstantDef.SEARCHEDTABLEID),
                                                 readpdbtable.getAField(
                    ConstantDef.SHOWFIELDID), "",
                                                 readpdbtable.getAField(
                    ConstantDef.SHOWFIELDID), true);
                if (!ret) { //第二级索引增加不上，忽略索引
                  m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
                  String tm = readpdbtable.getAField(ConstantDef.TABCOUNTID);
                  m_tabletabcount = Integer.parseInt(tm);
                  m_realtable = nodestr;
                  listTableInfo();
                }
              }
            }
            else { //无被检索表名
              m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
              m_tabletabcount = Integer.parseInt(
                  readpdbtable.getAField(ConstantDef.TABCOUNTID));
              m_realtable = nodestr;
              listTableInfo();
            }
          } //if !pdll
          else { //无索引标志
            m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
            m_tabletabcount = Integer.parseInt(
                readpdbtable.getAField(ConstantDef.TABCOUNTID));
            m_realtable = nodestr;
            listTableInfo();
          }
        }
        else {
          m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
          m_tabletabcount = Integer.parseInt(
              readpdbtable.getAField(ConstantDef.TABCOUNTID));
          m_realtable = nodestr;
          listTableInfo();

        }

        } //if buf
        else { //pdb_table中没有关于本表的设置
          m_orderfn = "";
          m_tabletabcount = 0;
          m_realtable = nodestr;
          listTableInfo();
        }
      }
    }
    else if (level == 3) { //分类索引名
      if (getModelDBType(m_node.getParent().getParent().toString()).trim().
          equalsIgnoreCase(
              ConstantDef.
              DEV)) {
        addVoltageNodes(m_node, nodestr);
      }
      else {
        m_db = getModelDB(m_node.getParent().getParent().toString()).trim();
        DefaultMutableTreeNode parentnode = (DefaultMutableTreeNode) m_node.
            getParent();
        Object parentnodeinfo = parentnode.getUserObject();
        String parentnodestr = parentnodeinfo.toString().trim(); //父节点名
        if (PreProcessor.ALIAS) {
          //如果是别名则需要获得实际表名
          String tablename = ApplicationDB.frameDB.GetTableNameFromAlias(
              parentnode.toString());
          if (tablename != "")
            parentnodestr = tablename;
        }

        //得到父节点的索引
        String wh = ConstantDef.TABLENAME + "='" + parentnodestr + "' and " +
            ConstantDef.SORTINDEX + ">0";

        //   //得到分类索引
        ReadPDBTable readpdbtable = new ReadPDBTable();
        int recs = readpdbtable.getPDBPropwhere(wh);

        if (recs > 0) { //有分类索引,在这一级的node肯定有索引，否则到不了这一步
          m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
          m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
          m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
          m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
          m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
          m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
          String searedtable = readpdbtable.getAField(ConstantDef.
              SEARCHEDTABLEID); //被检索表名
          if (searedtable.length() > 0) { //被检索表名,在这一级的"被检索表名"肯定有，否则到不了这一步
            String whnext = ConstantDef.TABLENAME + "='" + searedtable +
                "' and " +
                ConstantDef.SORTINDEX + ">0";
            //得到第二级分类索引
            ReadPDBTable readpdbtable2 = new ReadPDBTable();
            int recs2 = readpdbtable2.getPDBPropwhere(whnext);
            if (recs2 > 0 && !parentnodestr.equalsIgnoreCase(searedtable)) { //有第二级分类索引  searedtable!= parentnodestr
              int[] objdatatype = new int[1];
              byte[][] buflast;
              buflast = pdll.getAMatchFieldValue(m_db,
                                                 readpdbtable2.getAField(
                  ConstantDef.SEARCHEDTABLEID),
                                                 readpdbtable2.getAField(
                  ConstantDef.SEARCHEDFIELDID),
                                                 readpdbtable2.getAField(
                  ConstantDef.SHOWFIELDID),
                                                 nodestr, objdatatype);
              if (buflast.length > 0) {
                if (objdatatype[0] == ConstantDef.DATATYPE) {
                  wh = readpdbtable2.getAField(ConstantDef.SEARCHFIELDID) + "=" +
                      Integer.parseInt(pdll.substr(buflast[0]));
                }
                else if (objdatatype[0] == ConstantDef.FLOATTYPE) {
                  wh = readpdbtable2.getAField(ConstantDef.SEARCHFIELDID) + "=" +
                      Float.parseFloat(pdll.substr(buflast[0]));
                }
                else if (objdatatype[0] == ConstantDef.STRINGTYPE) {
                  wh = readpdbtable2.getAField(ConstantDef.SEARCHFIELDID) +
                      "='" +
                      pdll.substr(buflast[0]) + "'";
                }
              }
              else {
                wh = "";
              }
              pdll.addIndexNodes(m_node, m_db,
                                 readpdbtable.getAField(ConstantDef.
                  SEARCHEDTABLEID),
                                 readpdbtable.getAField(ConstantDef.SHOWFIELDID),
                                 wh,
                                 readpdbtable.getAField(ConstantDef.SHOWFIELDID), true);
            } //
            else { //没有第二级索引
              int[] objdatatype = new int[1];
              byte[][] buflast;
              buflast = pdll.getAMatchFieldValue(m_db,
                                                 readpdbtable.getAField(
                  ConstantDef.SEARCHEDTABLEID),
                                                 readpdbtable.getAField(
                  ConstantDef.SEARCHEDFIELDID),
                                                 readpdbtable.getAField(
                  ConstantDef.SHOWFIELDID),
                                                 nodestr, objdatatype);
              if (buflast.length > 0) {
                if (objdatatype[0] == ConstantDef.DATATYPE) {
                  wh = readpdbtable.getAField(ConstantDef.SEARCHFIELDID) + "=" +
                      Integer.parseInt(pdll.substr(buflast[0]));
                }
                else if (objdatatype[0] == ConstantDef.FLOATTYPE) {
                  wh = readpdbtable.getAField(ConstantDef.SEARCHFIELDID) + "=" +
                      Float.parseFloat(pdll.substr(buflast[0]));
                }
                else if (objdatatype[0] == ConstantDef.STRINGTYPE) {
                  wh = readpdbtable.getAField(ConstantDef.SEARCHFIELDID) + "='" +
                      pdll.substr(buflast[0]) + "'";
                }
              } //if buflast
              else {
                wh = "";
              }
              m_tablename = parentnodestr.trim();
              m_where = wh;
              m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
              m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
                  ConstantDef.
                  TABCOUNTID));
              m_realtable = parentnodestr;
              listTableInfo();
            } //else 没有第二级索引
          } //被检索表名
          else {
            return;
          }
        } //有分类索引
        else {
          return;
        }
      }
    }
    else if (level == 4) { //被分类索引的显示名
      if (getModelDBType(m_node.getParent().getParent().getParent().toString()).trim().
          equalsIgnoreCase(
              ConstantDef.
              DEV)) {
        String stationname = "";
        for (int i = 0; i < StationTable.length; i++) {
          if (StationTable[i].StationDescrible.equalsIgnoreCase(m_node.
              getParent().toString())) {
            stationname = StationTable[i].StationName;
            break;
          }
        }
        String wh = ConstantDef.SSCZ + "='" + stationname +"'";

       // addScdTypeNodes(m_node, nodestr);
       addSeparateNodes(m_node,wh);
      }
      else {

        m_db = getModelDB(m_node.getParent().getParent().getParent().toString()).
            trim();
        DefaultMutableTreeNode grandnode = (DefaultMutableTreeNode) m_node.
            getParent().getParent(); //祖父节点
        Object grandnodeinfo = grandnode.getUserObject();
        String grandnodestr = grandnodeinfo.toString().trim(); //祖父节点

        String wh = ConstantDef.TABLENAME + "='" + grandnodeinfo + "' and " +
            ConstantDef.SORTINDEX + ">0";
        ReadPDBTable readpdbtable = new ReadPDBTable();
        int recs = readpdbtable.getPDBPropwhere(wh);
        m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
        m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
        m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
        m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
        m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
        m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
        if (recs > 0) { //有分类索引
          int[] objdatatype = new int[1];
          byte[][] buflast; //得到检索域名的类型
          buflast = pdll.getAMatchFieldValue(m_db,
                                             readpdbtable.getAField(ConstantDef.
              SEARCHEDTABLEID),
                                             readpdbtable.getAField(ConstantDef.
              SEARCHEDFIELDID),
                                             readpdbtable.getAField(ConstantDef.
              SHOWFIELDID),
                                             nodestr, objdatatype);
          if (buflast.length > 0) {
            if (objdatatype[0] == ConstantDef.DATATYPE) {
              wh = readpdbtable.getAField(ConstantDef.SEARCHFIELDID) + "=" +
                  Integer.parseInt(pdll.substr(buflast[0]));
            }
            else if (objdatatype[0] == ConstantDef.FLOATTYPE) {
              wh = readpdbtable.getAField(ConstantDef.SEARCHFIELDID) + "=" +
                  Float.parseFloat(pdll.substr(buflast[0]));
            }
            else if (objdatatype[0] == ConstantDef.STRINGTYPE) {
              wh = readpdbtable.getAField(ConstantDef.SEARCHFIELDID) + "='" +
                  pdll.substr(buflast[0]) + "'";
            }
            m_tablename = grandnodestr.trim();
            m_where = wh;
            m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
            m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
                ConstantDef.
                TABCOUNTID));
            m_realtable = nodestr;
            listTableInfo();
          }
        }
      }
    }
    else if(level ==5){
      addScdTypeNodes(m_node, nodestr);
      /*
      String stationname = "";
         int voltageid =0;
         int scdtypeid =0;
         for (int i = 0; i < StationTable.length; i++) {
           if (StationTable[i].StationDescrible.equalsIgnoreCase(m_node.getParent().getParent().toString())) {
             stationname = StationTable[i].StationName;
             break;
           }
         }
         for (int i = 0; i < VoltageTable.length; i++) {
           if (VoltageTable[i].VoltageName.equalsIgnoreCase(m_node.
               getParent().toString())) {
             voltageid = VoltageTable[i].VoltageID;
             break;
           }
         }
         for (int i = 0; i < ScdTypeTable.length; i++) {
           if (ScdTypeTable[i].ScdTypeName.equalsIgnoreCase(m_node.toString())) {
             scdtypeid = ScdTypeTable[i].ScdTypeID;
             break;
           }
         }
         String wh = ConstantDef.ZSXXDM + "='" + stationname + "' and " +
         ConstantDef.LX + "=" + scdtypeid + " and " + ConstantDef.DYDJ + "=" + voltageid;

      addDevNodes(m_node, wh);
      /*
      m_db = getModelDB(m_node.getParent().getParent().getParent().getParent().
                       toString()).trim();
      String tablename = "SCD设备参数表";
      String wh = ConstantDef.TABLENAME + "='" + tablename + "' and " +
          ConstantDef.SORTINDEX + ">0";
      ReadPDBTable readpdbtable = new ReadPDBTable();
      int recs = readpdbtable.getPDBPropwhere(wh);
      m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
      m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
      m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
      m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
      m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
      m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
      if (recs > 0) { //有分类索引
          String stationname = "";
          int voltageid =0;
          int scdtypeid =0;
          for (int i = 0; i < StationTable.length; i++) {
            if (StationTable[i].StationDescrible.equalsIgnoreCase(m_node.getParent().getParent().toString())) {
              stationname = StationTable[i].StationName;
              break;
            }
          }
          for (int i = 0; i < VoltageTable.length; i++) {
            if (VoltageTable[i].VoltageName.equalsIgnoreCase(m_node.
                getParent().toString())) {
              voltageid = VoltageTable[i].VoltageID;
              break;
            }
          }
          for (int i = 0; i < ScdTypeTable.length; i++) {
            if (ScdTypeTable[i].ScdTypeName.equalsIgnoreCase(m_node.toString())) {
              scdtypeid = ScdTypeTable[i].ScdTypeID;
              break;
            }
          }
          wh = ConstantDef.ZSXXDM + "='" + stationname + "' and " +
          ConstantDef.LX + "=" + scdtypeid + " and " + ConstantDef.DYDJ + "=" + voltageid;

          m_tablename = tablename;
          m_where = wh;
          m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
          m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
              ConstantDef.
              TABCOUNTID));
          m_realtable = tablename;
          listTableInfo();
        }
       */
    }
    else if(level == 6){
      /*
      DefaultMutableTreeNode nodetype = new DefaultMutableTreeNode(
          "遥信参数表");
      m_node.add(nodetype);
      DefaultMutableTreeNode nodetype1 = new DefaultMutableTreeNode(
          "遥测参数表");
      m_node.add(nodetype1);
      DefaultMutableTreeNode nodetype2 = new DefaultMutableTreeNode(
          "电度参数表");
      m_node.add(nodetype2);
     */
    String stationname = "";
         int voltageid =0;
         int scdtypeid =0;
         int separateid=0;
         for (int i = 0; i < StationTable.length; i++) {
           if (StationTable[i].StationDescrible.equalsIgnoreCase(m_node.getParent().getParent().getParent().toString())) {
             stationname = StationTable[i].StationName;
             break;
           }
         }
         for (int i = 0; i < VoltageTable.length; i++) {
           if (VoltageTable[i].VoltageName.equalsIgnoreCase(m_node.
               getParent().getParent().toString())) {
             voltageid = VoltageTable[i].VoltageID;
             break;
           }
         }
         for (int i = 0; i < ScdTypeTable.length; i++) {
           if (ScdTypeTable[i].ScdTypeName.equalsIgnoreCase(m_node.toString())) {
             scdtypeid = ScdTypeTable[i].ScdTypeID;
             break;
           }
         }
         for (int i = 0; i < Separate.length; i++) {
           if (Separate[i].SeparateDesc.equalsIgnoreCase(m_node.getParent().toString())) {
             separateid = Separate[i].SeparateID;
             break;
           }
         }

         String wh = ConstantDef.ZSXXDM + "='" + stationname + "' and " +
         ConstantDef.LX + "=" + scdtypeid + " and " + ConstantDef.DYDJ + "=" + voltageid + " and " +  ConstantDef.SEPARATE + "="  + separateid ;

      addDevNodes(m_node, wh);

    }
    else if (level == 7) {
      /*
      m_db = "modeldb";
      String tablename = m_node.toString();
      String wh = ConstantDef.TABLENAME + "='" + tablename + "' and " +
          ConstantDef.SORTINDEX + ">0";
      ReadPDBTable readpdbtable = new ReadPDBTable();
      int recs = readpdbtable.getPDBPropwhere(wh);
      m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
      m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
      m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
      m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
      m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
      m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
      if (recs > 0) { //有分类索引
        String devname = "";
        for (int i = 0; i < DevTable.length; i++) {
          if (DevTable[i].DevDescrible.equalsIgnoreCase(m_node.
              getParent().toString())) {
            devname = DevTable[i].DevName;
            break;
          }
        }
        wh = ConstantDef.SBDM + " = '" + devname + "'";

        m_tablename = tablename;
        m_where = wh;
        m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
        m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
            ConstantDef.
            TABCOUNTID));
        m_realtable = tablename;
        listTableInfo();
      }

*/
DefaultMutableTreeNode nodetype = new DefaultMutableTreeNode(
   "遥信参数表");
m_node.add(nodetype);
DefaultMutableTreeNode nodetype1 = new DefaultMutableTreeNode(
   "遥测参数表");
m_node.add(nodetype1);
DefaultMutableTreeNode nodetype2 = new DefaultMutableTreeNode(
   "电度参数表");
m_node.add(nodetype2);

    }
    else if(level == 8){
      m_db = "modeldb";
      String tablename = m_node.toString();
      String wh = ConstantDef.TABLENAME + "='" + tablename + "' and " +
          ConstantDef.SORTINDEX + ">0";
      ReadPDBTable readpdbtable = new ReadPDBTable();
      int recs = readpdbtable.getPDBPropwhere(wh);
      m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
      m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
      m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
      m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
      m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
      m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
      if (recs > 0) { //有分类索引
        String devname = "";
        for (int i = 0; i < DevTable.length; i++) {
          if (DevTable[i].DevDescrible.equalsIgnoreCase(m_node.
              getParent().toString())) {
            devname = DevTable[i].DevName;
            break;
          }
        }
        wh = ConstantDef.SBDM + " = '" + devname + "'";

        m_tablename = tablename;
        m_where = wh;
        m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
        m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
            ConstantDef.
            TABCOUNTID));
        m_realtable = tablename;
        listTableInfo();
      }

    }

  }
  //CIM模型专用 -hx 编辑区域参数表

  public void OpenArea(DefaultMutableTreeNode nodepara, String nodestr,
                             int level){
      m_node= nodepara;
      m_db = "modeldb";
      String title = m_node.toString().trim() + "/" + "区域信息";
      String tablename = "区域参数表";
      String wh = ConstantDef.TABLENAME + "='" + tablename + "'" ;
      ReadPDBTable readpdbtable = new ReadPDBTable();
      int recs = readpdbtable.getPDBPropwhere(wh);
      m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
      m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
      m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
      m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
      m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
      m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
      if (recs > 0) { //有分类索引

        m_tablename = tablename;
        m_where = "";
        m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
        m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
            ConstantDef.
            TABCOUNTID));
        m_realtable = tablename;
        listCIMTableInfo(title);
      }
}
    //CIM模型专用 -hx 编辑站所线系参数表

    public void OpenStation(DefaultMutableTreeNode nodepara, String nodestr,
                               int level){
        m_node= nodepara;
        m_db = "modeldb";
        String tablename = "站所线系参数表";
        String wh = ConstantDef.TABLENAME + "='" + tablename + "'" ;
        String title = m_node.toString().trim() + "/" + "站所信息";


        ReadPDBTable readpdbtable = new ReadPDBTable();
        int recs = readpdbtable.getPDBPropwhere(wh);
        m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
        m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
        m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
        m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
        m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
        m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
        if (recs > 0) { //有分类索引

          m_tablename = tablename;
          m_where = "";
          m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
          m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
              ConstantDef.
              TABCOUNTID));
          m_realtable = tablename;
          listCIMTableInfo(title);
        }
}
      //CIM模型专用 -hx 编辑当前区域下的站所线系参数表

      public void OpenAStation(DefaultMutableTreeNode nodepara, String nodestr,
                              int level) {
        int AreaID = 0;
        for (int i = 0; i < AreaTable.length; i++) {
          if (AreaTable[i].AreaDescrible.equalsIgnoreCase(nodepara.toString())) {
            AreaID = AreaTable[i].AreaID;
            break;
          }

        }
        String wh1 = ConstantDef.AreaID + "=" + AreaID;

        m_node = nodepara;
        m_db = "modeldb";
        String title = m_node.toString().trim() + "/" + "站所信息";
        DefaultMutableTreeNode node1 = m_node;
        for (int i = 2; i < level; i++) {
          node1 = (DefaultMutableTreeNode) node1.getParent();
          title = node1.toString().trim() + "/" + title;
        }

        String tablename = "站所线系参数表";
        String wh = ConstantDef.TABLENAME + "='" + tablename + "'";

        ReadPDBTable readpdbtable = new ReadPDBTable();
        int recs = readpdbtable.getPDBPropwhere(wh);
        m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
        m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
        m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
        m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
        m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
        m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
        if (recs > 0) { //有分类索引

          m_tablename = tablename;
          m_where = wh1;
          m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
          m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
              ConstantDef.
              TABCOUNTID));
          m_realtable = tablename;
          listCIMTableInfo(title);
        }
      }

//CIM模型专用 -hx 编辑电压等级参数表

      public void OpenVoltage(DefaultMutableTreeNode nodepara, String nodestr,
                                 int level){
          m_node= nodepara;
          m_db = "modeldb";
          String title = m_node.toString().trim() + "/" + "电压等级";
          DefaultMutableTreeNode node1 = m_node;
          for (int i = 2; i < level; i++) {
            node1 = (DefaultMutableTreeNode) node1.getParent();
            title = node1.toString().trim() + "/" + title;
          }

          String tablename = "电压等级表";
          String wh = ConstantDef.TABLENAME + "='" + tablename + "'" ;
          ReadPDBTable readpdbtable = new ReadPDBTable();
          int recs = readpdbtable.getPDBPropwhere(wh);
          m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
          m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
          m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
          m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
          m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
          m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
          if (recs > 0) { //有分类索引

            m_tablename = tablename;
            m_where = "";
            m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
            m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
                ConstantDef.
                TABCOUNTID));
            m_realtable = tablename;
            listCIMTableInfo(title);
          }
}
//CIM模型专用 -hx 编辑当前厂站下的SCD参数表
 public void OpenSDev(DefaultMutableTreeNode nodepara, String nodestr,
                           int level){
   String stationname="";
    m_node= nodepara;
    m_db = "modeldb";
    String tablename = "SCD设备参数表";
    String wh = ConstantDef.TABLENAME + "='" + tablename + "'" ;
    for (int i = 0; i < StationTable.length; i++) {
      if (StationTable[i].StationDescrible.equalsIgnoreCase(m_node.toString())) {
        stationname = StationTable[i].StationName;
        break;
      }
    }
    String title = m_node.toString().trim() + "/" + "设备参数";
    DefaultMutableTreeNode node1 = m_node;
    for (int i = 2; i < level; i++) {
      node1 = (DefaultMutableTreeNode) node1.getParent();
      title = node1.toString().trim() + "/" + title;
    }

    ReadPDBTable readpdbtable = new ReadPDBTable();
    int recs = readpdbtable.getPDBPropwhere(wh);
    m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
    m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
    m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
    m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
    m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
    m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
    if (recs > 0) { //有分类索引

      m_tablename = tablename;
      m_where = ConstantDef.ZSXXDM + "='" + stationname + "'";
      m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
      m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
          ConstantDef.
          TABCOUNTID));
      m_realtable = tablename;
      listCIMTableInfo(title);
    }
}
  //CIM模型专用 -hx 编辑间隔参数表
 public void OpenSeparate(DefaultMutableTreeNode nodepara, String nodestr,
                           int level){
    m_node= nodepara;
    m_db = "modeldb";
    String tablename = "间隔参数表";
    String wh = ConstantDef.TABLENAME + "='" + tablename + "'" ;
    String title = m_node.toString().trim() + "/" + "间隔参数表";
    DefaultMutableTreeNode node1 = m_node;
    for (int i = 2; i < level; i++) {
      node1 = (DefaultMutableTreeNode) node1.getParent();
      title = node1.toString().trim() + "/" + title;
    }

    ReadPDBTable readpdbtable = new ReadPDBTable();
    int recs = readpdbtable.getPDBPropwhere(wh);
    m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
    m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
    m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
    m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
    m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
    m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
    if (recs > 0) { //有分类索引

      m_tablename = tablename;
      m_where = "";
      m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
      m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
          ConstantDef.
          TABCOUNTID));
      m_realtable = tablename;
      listCIMTableInfo(title);
    }
    else
    {
      JOptionPane.showMessageDialog(null, "参数表中无此表的信息!", "操作提示信息",
                                            JOptionPane.INFORMATION_MESSAGE);

    }
}

  //CIM模型专用 -hx 编辑设备类型参数表
   public void OpenDevType(DefaultMutableTreeNode nodepara, String nodestr,
                             int level){
      m_node= nodepara;
      m_db = "modeldb";
      String tablename = "SCD设备类型表";
      String wh = ConstantDef.TABLENAME + "='" + tablename + "'" ;
      String title = m_node.toString().trim() + "/" + "设备类型";
      DefaultMutableTreeNode node1 = m_node;
      for (int i = 2; i < level; i++) {
        node1 = (DefaultMutableTreeNode) node1.getParent();
        title = node1.toString().trim() + "/" + title;
      }

      ReadPDBTable readpdbtable = new ReadPDBTable();
      int recs = readpdbtable.getPDBPropwhere(wh);
      m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
      m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
      m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
      m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
      m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
      m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
      if (recs > 0) { //有分类索引

        m_tablename = tablename;
        m_where = "";
        m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
        m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
            ConstantDef.
            TABCOUNTID));
        m_realtable = tablename;
        listCIMTableInfo(title);
      }
      else
      {
        JOptionPane.showMessageDialog(null, "参数表中无此表的信息!", "操作提示信息",
                                              JOptionPane.INFORMATION_MESSAGE);

      }
}
//打开当前厂站当前电压等级下的所有SCD设备
public void OpenSVDev(DefaultMutableTreeNode nodepara, String nodestr,
                           int level){
  String stationname = "";
  m_node= nodepara;
  m_db = "modeldb";
  int voltageid = 0;
  for (int i = 0; i < StationTable.length; i++) {
    if (StationTable[i].StationDescrible.equalsIgnoreCase(m_node.getParent().toString())) {
      stationname = StationTable[i].StationName;
      break;
    }
  }
  for (int i = 0; i < VoltageTable.length; i++) {
    if (VoltageTable[i].VoltageName.equalsIgnoreCase(m_node.toString())) {
      voltageid = VoltageTable[i].VoltageID;
      break;
    }
  }
  String wh1 = ConstantDef.ZSXXDM + "='" + stationname + "' and "+ ConstantDef.DYDJ + "=" +
      voltageid;


    String title = m_node.toString().trim() + "/" + "设备参数";
    DefaultMutableTreeNode node1 = m_node;
    for (int i = 2; i < level; i++) {
      node1 = (DefaultMutableTreeNode) node1.getParent();
      title = node1.toString().trim() + "/" + title;
    }

    String tablename = "SCD设备参数表";
    String wh = ConstantDef.TABLENAME + "='" + tablename + "'" ;

    ReadPDBTable readpdbtable = new ReadPDBTable();
    int recs = readpdbtable.getPDBPropwhere(wh);
    m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
    m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
    m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
    m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
    m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
    m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
    if (recs > 0) { //有分类索引

      m_tablename = tablename;
      m_where = wh1;
      m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
      m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
          ConstantDef.
          TABCOUNTID));
      m_realtable = tablename;
      listCIMTableInfo(title);
    }
}
  //打开当前厂站当前电压等级下当前间隔的所有SCD设备
public void OpenSeparateDev(DefaultMutableTreeNode nodepara, String nodestr,
                           int level){
  String stationname = "";
  m_node= nodepara;
  m_db = "modeldb";
  int voltageid = 0;
  int separateid=0;
  for (int i = 0; i < StationTable.length; i++) {
    if (StationTable[i].StationDescrible.equalsIgnoreCase(m_node.getParent().getParent().toString())) {
      stationname = StationTable[i].StationName;
      break;
    }
  }
  for (int i = 0; i < VoltageTable.length; i++) {
    if (VoltageTable[i].VoltageName.equalsIgnoreCase(m_node.getParent().toString())) {
      voltageid = VoltageTable[i].VoltageID;
      break;
    }
  }
  for (int i = 0; i < Separate.length; i++) {
  if (Separate[i].SeparateDesc.equalsIgnoreCase(m_node.toString())) {
    separateid = Separate[i].SeparateID;
    break;
  }
}

  String wh1 = ConstantDef.ZSXXDM + "='" + stationname + "' and "+ ConstantDef.DYDJ + "=" +
      voltageid + " and " + ConstantDef.SEPARATE + "=" + separateid;


    String title = m_node.toString().trim() + "/" + "设备参数";
    DefaultMutableTreeNode node1 = m_node;
    for (int i = 2; i < level; i++) {
      node1 = (DefaultMutableTreeNode) node1.getParent();
      title = node1.toString().trim() + "/" + title;
    }

    String tablename = "SCD设备参数表";
    String wh = ConstantDef.TABLENAME + "='" + tablename + "'" ;

    ReadPDBTable readpdbtable = new ReadPDBTable();
    int recs = readpdbtable.getPDBPropwhere(wh);
    m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
    m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
    m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
    m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
    m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
    m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
    if (recs > 0) { //有分类索引

      m_tablename = tablename;
      m_where = wh1;
      m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
      m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
          ConstantDef.
          TABCOUNTID));
      m_realtable = tablename;
      listCIMTableInfo(title);
    }
}

  //打开当前厂站当前电压等级当前类型下的所有SCD设备
  public void OpenSVTDev(DefaultMutableTreeNode nodepara, String nodestr,
                             int level){
    m_node= nodepara;
    m_db = "modeldb";
  String stationname = "";
  String title="";
   int voltageid =0;
   int scdtypeid =0;
   for (int i = 0; i < StationTable.length; i++) {
     if (StationTable[i].StationDescrible.equalsIgnoreCase(m_node.getParent().getParent().toString())) {
       stationname = StationTable[i].StationName;
       break;
     }
   }
   for (int i = 0; i < VoltageTable.length; i++) {
     if (VoltageTable[i].VoltageName.equalsIgnoreCase(m_node.
         getParent().toString())) {
       voltageid = VoltageTable[i].VoltageID;
       break;
     }
   }
   for (int i = 0; i < ScdTypeTable.length; i++) {
     if (ScdTypeTable[i].ScdTypeName.equalsIgnoreCase(m_node.toString())) {
       scdtypeid = ScdTypeTable[i].ScdTypeID;
       break;
     }
   }
   String wh1 = ConstantDef.ZSXXDM + "='" + stationname + "' and " +
   ConstantDef.LX + "=" + scdtypeid + " and " + ConstantDef.DYDJ + "=" + voltageid;




      title = m_node.toString().trim() + "/"+ "设备参数";
      DefaultMutableTreeNode node1 = m_node;
      for (int i = 2; i < level; i++) {
        node1 = (DefaultMutableTreeNode) node1.getParent();
        title = node1.toString().trim() + "/" + title;
      }

      String tablename = "SCD设备参数表";
      String wh = ConstantDef.TABLENAME + "='" + tablename + "'" ;

      ReadPDBTable readpdbtable = new ReadPDBTable();
      int recs = readpdbtable.getPDBPropwhere(wh);
      m_vTabnoName.add(0, readpdbtable.getAField(ConstantDef.TAB1ID));
      m_vTabnoName.add(1, readpdbtable.getAField(ConstantDef.TAB2ID));
      m_vTabnoName.add(2, readpdbtable.getAField(ConstantDef.TAB3ID));
      m_vTabnoName.add(3, readpdbtable.getAField(ConstantDef.TAB4ID));
      m_vTabnoName.add(4, readpdbtable.getAField(ConstantDef.TAB5ID));
      m_slistfield = readpdbtable.getAField(ConstantDef.LISTFIELDID);
      if (recs > 0) { //有分类索引

        m_tablename = tablename;
        m_where = wh1;
        m_orderfn = readpdbtable.getAField(ConstantDef.SORTFIELDID);
        m_tabletabcount = Integer.parseInt(readpdbtable.getAField(
            ConstantDef.
            TABCOUNTID));
        m_realtable = tablename;
        listCIMTableInfo(title);
      }
}
//CIM模型专用 ---打开表
    public void listCIMTableInfo(String title1) {
  if (m_tablename == null || m_tablename == "") {
    return;
  }
  int tabcount = jtab.getTabCount();
  int i = 0;
  int tabindex = 9999;
  int level = m_node.getLevel();
  int tab = 0;
  //-------------------
  /*
  String title1 = "";
  title1 = m_node.toString().trim();
  DefaultMutableTreeNode node1 = m_node;
  for (i = 2; i < level; i++) {
    node1 = (DefaultMutableTreeNode) node1.getParent();
    title1 = node1.toString().trim() + "/" + title1;
  }
*/
//---------------------


  for (i = 0; i < tabcount; i++) {
    String tabtitle = jtab.getTitleAt(i).trim(); //用全路径
    if (tabtitle.equalsIgnoreCase(title1) == true) {
      tabindex = i;
      break;
    }
  }
  if (i >= tabcount) { //当前显示中没有该表


    if (m_tabletabcount > 5) {
      m_tabletabcount = 5; //分区最多5个
    }
    if (m_tabletabcount <= 0) {
      m_tabletabcount = 1; //分区数至少为1
    }
    tabindex = i;
    vKey.removeAllElements(); //要清除，否则以后就得不到正确的结果
    vTabAreaData.removeAllElements(); //清除前一表的所有数据
    vColumnClass.removeAllElements();

    m_managemodel = new ManageModel();
    //获得表内容
    int fieldnums = getTableContent(tabindex);

    if (fieldnums < 1) {
      return;
    }
    //------hongxiang
    String title2 = "";
    title2 = title2 + m_node.toString().trim();
    DefaultMutableTreeNode node2 = m_node;
    for (int t = 0; t < level; t++) {
      node2 = (DefaultMutableTreeNode) node2.getParent();
      title2 = node2.toString().trim() + "/" + title2;
    }
    title2 = "     树节点路径:/" + title2;
    vTitle.addElement(title2);

//--------------
//vTitle

    vTableTabnoCount.add(tabindex, Integer.toString(m_tabletabcount)); //保存每个表的分区数
    JScrollPane jscrolltab = new JScrollPane();
    jscrolltab.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                       ConstantDef.CBLUE));
    int realTabnoCount = Integer.parseInt(vTableTabnoCount.elementAt(
        tabindex).toString().trim()); //实际分区数
    YKInfo ykinfo = new YKInfo();

    vOpenLeafNode.add(m_node);

    m_curtablename = title1;
    if (realTabnoCount == 1) { //实际分区数为1

      vTabnoJTab.add(tabindex, Integer.toString(0));
      MyTableModel myTableModel = new MyTableModel( (Object[][]) vTabAreaData.
          elementAt(1), (Object[]) vTabAreaData.elementAt(0)); //只有1个分区的数据分别放在0,1两个位置

      m_managemodel.initFlag(myTableModel.getRowCount());

      myTableModel.setManageModel(m_managemodel);

      myTableModel.setTableType(tablesetinfo.tableFieldInfo.getTableType());

      myTableModel.setDBName(m_db, m_tablename);

      myTableModel.setKey(vKey);

      myTableModel.setColType( (Vector) vColumnClass.elementAt(0));
      myTableModel.setLongValues( (Vector) vLongValues.elementAt(0));
      myTableModel.setListField(m_slistfield, m_vlistfield);
      myTableModel.init();
      myTableModel.initEnds();

      MyJTable listtable = new MyJTable(myTableModel);
      vListTable.add(i, listtable);

      jscrolltab.getViewport().add(listtable, null);

      listtable.setColCount(myTableModel.getColumnCount());
      listtable.setFontSize(fontsize);
      listtable.setTableSetInfo(tablesetinfo);
      listtable.setRealTable(m_realtable);
      listtable.setWhere(m_where);
      listtable.setPDBField(pdb_fieldtable);
      listtable.setScrollTab(jscrolltab);
      listtable.setNode(m_node);
      listtable.setRowHeight(rowheight);
      listtable.setCurTabno(0);
      listtable.setFirstJTable(listtable);

      MyJTableHeader myjtableheader = new MyJTableHeader(listtable.
          getColumnModel());
      myjtableheader.setTable(listtable);
      myjtableheader.init();
      listtable.setTableHeader(myjtableheader);
      listtable.init();
      if (m_tablename.equals("遥信参数表") || m_tablename.equals("遥测参数表") ||
          m_tablename.equals("电度参数表") || m_tablename.equals("SCD设备参数表") ||
          m_tablename.equals("终端参数表")) {
        for (int t = 0; t < fieldnums; t++) {
          String code = myTableModel.columnIdentifiers.elementAt(t).toString();
          if (code.equalsIgnoreCase("代码")) {
            //  tab=t;
            //   listtable.getColumnModel().getColumn(tab).setPreferredWidth(0);
          }
        }
      }
      if (ykinfo.isYXTable(m_tablename)) {
        ykinfo.setYKSomeField(myTableModel);
        myTableModel.setYKInfo(ykinfo);
      }

      jtab.addTab(title1, imagetab, jscrolltab);
      jtab.setToolTipTextAt(tabindex, m_tablename);

      myTableModel.addTableModelListener(new javax.swing.event.
                                         TableModelListener() {
        public void tableChanged(TableModelEvent e) {
          //表内容修改后的操作、标志等
          int tabindex1 = jtab.getSelectedIndex();
          if (tabindex1 >= 0) {
            MyTableModel myTableModel1 = (MyTableModel) ( (MyJTable)
                vListTable.elementAt(tabindex1)).getModel();
            if (myTableModel1.getCellChangedFlag()) {
              myTableModel1.setCellChangedFlag(false);
              jtab.setForegroundAt(tabindex1, Color.red);
            }
          } //if indextab>=0
        }
      });
      //处理ESCAPE、DELETE键
      listtable.addKeyListener(new java.awt.event.KeyAdapter() {
        public void keyPressed(KeyEvent e) {
          doPressKey(e);
        } //public void keyTyped(KeyEvent e) {
      });

    } //if(ralTabnoCount == 1)
    else { //有多个分区
      final JTabbedPane jtabbedPaneTabno = new JTabbedPane();
      vTabnoJTab.add(tabindex, jtabbedPaneTabno);
      jtabbedPaneTabno.setTabPlacement(JTabbedPane.BOTTOM);
      jtabbedPaneTabno.setBackground(new Color(ConstantDef.CRED,
                                               ConstantDef.CGREEN,
                                               ConstantDef.CBLUE));
      jscrolltab.getViewport().add(jtabbedPaneTabno, null); //增加分区的tab容器
      MyJTable firstlisttable = null;
      int colcount = 0; //总的列数

      int setpvalue = 10 / realTabnoCount;
      int count = 1;
      for (int m = 0; m < realTabnoCount; m++) {
        MyTableModel myTableModel = new MyTableModel((Object[][])
            vTabAreaData.elementAt(m * 2 + 1),
            (Object[]) vTabAreaData.elementAt(m * 2));

        myTableModel.setColType( (Vector) vColumnClass.elementAt(m));
        myTableModel.setLongValues( (Vector) vLongValues.elementAt(m));
        myTableModel.setTableType(tablesetinfo.tableFieldInfo.getTableType()); //vTableType.elementAt(tabindex).toString());
        myTableModel.setDBName(m_db, m_tablename);
        myTableModel.setKey(vKey);
        myTableModel.setListField(m_slistfield, m_vlistfield);
        myTableModel.init();
        myTableModel.initEnds();
        colcount += myTableModel.getColumnCount();
        MyJTable listtable = new MyJTable(myTableModel);

        if (m == 0) {
          vListTable.add(i, listtable);
          firstlisttable = listtable;
          m_managemodel.initFlag(myTableModel.getRowCount());
        }
        else {
          firstlisttable.addJTable(listtable);
        }

        myTableModel.setManageModel(m_managemodel);
        listtable.setFirstJTable(firstlisttable);
        JScrollPane jscrolltabnotab = new JScrollPane();

        listtable.setFontSize(fontsize);
        listtable.setTableSetInfo(tablesetinfo);
        listtable.setRealTable(m_realtable);
        listtable.setWhere(m_where);
        listtable.setPDBField(pdb_fieldtable);
        listtable.setCurTabno(m);
        listtable.setScrollTab(jscrolltabnotab);
        listtable.setNode(m_node);
        listtable.setRowHeight(rowheight);

        MyJTableHeader myjtableheader = new MyJTableHeader(listtable.
            getColumnModel());
        myjtableheader.setTable(listtable);
        myjtableheader.init();
        listtable.setTableHeader(myjtableheader);

        listtable.init();
        if (m_tablename.equals("遥信参数表") || m_tablename.equals("遥测参数表") ||
            m_tablename.equals("电度参数表") || m_tablename.equals("SCD设备参数表") ||
            m_tablename.equals("终端参数表")) {
          for (int t = 0; t < myTableModel.columnIdentifiers.size(); t++) {
            String code = myTableModel.columnIdentifiers.elementAt(t).
                toString();
            int dfs = 0;
            if (code.equalsIgnoreCase("代码")) {
              //  tab=t;
              //  listtable.getColumnModel().getColumn(tab).setPreferredWidth(0);
            }
          }
        }

        if (ykinfo.isYXTable(m_tablename)) {
          ykinfo.setYKSomeField(myTableModel);
        }

        jtabbedPaneTabno.add(m_vTabnoName.elementAt(m).toString(),
                             jscrolltabnotab);
        jscrolltabnotab.getViewport().add(listtable, null);

        listtable.addKeyListener(new java.awt.event.KeyAdapter() {
          public void keyPressed(KeyEvent e) {
            doPressKey(e);
          } //public void keyTyped(KeyEvent e) {
        });
        myTableModel.addTableModelListener(new javax.swing.event.
                                           TableModelListener() {
          public void tableChanged(TableModelEvent e) {
            //表内容修改后的操作、标志等
            int tabindex1 = jtab.getSelectedIndex();
            int tabindex2 = jtabbedPaneTabno.getSelectedIndex();
            MyJTable myjtable = null;
            MyTableModel myTableModel1 = null;
            if (tabindex1 >= 0 && tabindex2 >= 0) {
              myjtable = (MyJTable) vListTable.elementAt(tabindex1);
              if (tabindex2 == 0) {
                myTableModel1 = (MyTableModel) myjtable.getModel();
              }
              else {
                MyJTable jtabtmp = myjtable.getJTable(tabindex2 - 1);
                myTableModel1 = (MyTableModel) jtabtmp.getModel();
              }
              if (myTableModel1.getCellChangedFlag()) {
                myTableModel1.setCellChangedFlag(false);
                jtab.setForegroundAt(tabindex1, Color.red);
              }
            } //if indextab>=0
          }
        });
      } //for m
      if (ykinfo.isYXTable(m_tablename)) {
        MyTableModel mymodelyk = (MyTableModel) firstlisttable.getModel();
        mymodelyk.setYKInfo(ykinfo);
        for (int k = 0; k < realTabnoCount - 1; k++) {
          mymodelyk = (MyTableModel) ( (MyJTable) firstlisttable.getJTable(k)).
              getModel();
          mymodelyk.setYKInfo(ykinfo);
        }
      }
      firstlisttable.setColCount(colcount);
      jtab.addTab(title1, imagetab, jscrolltab);
      jtab.setToolTipTextAt(tabindex, m_tablename);

      jtabbedPaneTabno.addKeyListener(new java.awt.event.KeyAdapter() {
        public void keyPressed(KeyEvent e) {
          doPressKey(e);
        } //public void keyPressed(KeyEvent e) {
      });
    } //else 有多个分区
  } //if i
  if (jtab.getTabCount() > tabindex) {
    jtab.setSelectedIndex(tabindex);
  }
}

//-------模糊过滤重新过滤表 start
//用户点击树节点后需要打开显示表
 public void listfuzzyTableInfo() {
   if (m_tablename == null || m_tablename == "") {
     return;
   }
   int tabcount = jtab.getTabCount();
   int i = 0;
   int tabindex = 9999;
   int level = m_node.getLevel();
   int tab = 0;
   String title1 = "";
   title1 = m_node.toString().trim();
   DefaultMutableTreeNode node1 = m_node;
   if(m_node.getLevel()==7){
     for (i = 6; i < level; i++) {
       node1 = (DefaultMutableTreeNode) node1.getParent();
       title1 = node1.toString().trim() + "/" + title1;
     }

   }
   else{
     for (i = 2; i < level; i++) {
       node1 = (DefaultMutableTreeNode) node1.getParent();
       title1 = node1.toString().trim() + "/" + title1;
     }
   }
   /*
   for (i = 0; i < tabcount; i++) {
     String tabtitle = jtab.getTitleAt(i).trim(); //用全路径
     if (tabtitle.equalsIgnoreCase(title1) == true) {
       tabindex = i;
       break;
     }
   }
*/
   if (i >= tabcount) { //当前显示中没有该表

     if (m_tabletabcount > 5) {
       m_tabletabcount = 5; //分区最多5个
     }
     if (m_tabletabcount <= 0) {
       m_tabletabcount = 1; //分区数至少为1
     }
     tabindex = i;
     vKey.removeAllElements(); //要清除，否则以后就得不到正确的结果
     vTabAreaData.removeAllElements(); //清除前一表的所有数据
     vColumnClass.removeAllElements();
     vTableTabnoCount.removeAllElements();

     m_managemodel = new ManageModel();
     //获得表内容
     int fieldnums = getTableContent(tabindex);

     if (fieldnums < 1) {
       return;
     }
     //------hongxiang
     String title2 = "";
     title2 = title2 + m_node.toString().trim();
     DefaultMutableTreeNode node2 = m_node;
     for (int t = 0; t < level; t++) {
       node2 = (DefaultMutableTreeNode) node2.getParent();
       title2 = node2.toString().trim() + "/" + title2;
     }
     title2 = "     树节点路径:/" + title2;
     vTitle.addElement(title2);

//--------------
//vTitle

     vTableTabnoCount.add(tabindex, Integer.toString(m_tabletabcount)); //保存每个表的分区数
     JScrollPane jscrolltab = new JScrollPane();
     jscrolltab.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                        ConstantDef.CBLUE));
     int realTabnoCount = Integer.parseInt(vTableTabnoCount.elementAt(
         tabindex).toString().trim()); //实际分区数
     YKInfo ykinfo = new YKInfo();

     vOpenLeafNode.add(m_node);

     m_curtablename = title1;
     if (realTabnoCount == 1) { //实际分区数为1

       vTabnoJTab.add(tabindex, Integer.toString(0));
       MyTableModel myTableModel = new MyTableModel( (Object[][]) vTabAreaData.
           elementAt(1), (Object[]) vTabAreaData.elementAt(0)); //只有1个分区的数据分别放在0,1两个位置

       m_managemodel.initFlag(myTableModel.getRowCount());

       myTableModel.setManageModel(m_managemodel);

       myTableModel.setTableType(tablesetinfo.tableFieldInfo.getTableType());

       myTableModel.setDBName(m_db, m_tablename);

       myTableModel.setKey(vKey);

       myTableModel.setColType( (Vector) vColumnClass.elementAt(0));
       myTableModel.setLongValues( (Vector) vLongValues.elementAt(0));
       myTableModel.setListField(m_slistfield, m_vlistfield);
       myTableModel.init();
       myTableModel.initEnds();

       MyJTable listtable = new MyJTable(myTableModel);
       vListTable.add(i, listtable);

       jscrolltab.getViewport().add(listtable, null);

       listtable.setColCount(myTableModel.getColumnCount());
       listtable.setFontSize(fontsize);
       listtable.setTableSetInfo(tablesetinfo);
       listtable.setRealTable(m_realtable);
       listtable.setWhere(m_where);
       listtable.setPDBField(pdb_fieldtable);
       listtable.setScrollTab(jscrolltab);
       listtable.setNode(m_node);
       listtable.setRowHeight(rowheight);
       listtable.setCurTabno(0);
       listtable.setFirstJTable(listtable);

       MyJTableHeader myjtableheader = new MyJTableHeader(listtable.
           getColumnModel());
       myjtableheader.setTable(listtable);
       myjtableheader.init();
       listtable.setTableHeader(myjtableheader);
       listtable.init();
       if (m_tablename.equals("遥信参数表") || m_tablename.equals("遥测参数表") ||
           m_tablename.equals("电度参数表") || m_tablename.equals("SCD设备参数表") ||
           m_tablename.equals("终端参数表")) {
         for (int t = 0; t < fieldnums; t++) {
           String code = myTableModel.columnIdentifiers.elementAt(t).toString();
           if (code.equalsIgnoreCase("代码")) {
             //  tab=t;
             //   listtable.getColumnModel().getColumn(tab).setPreferredWidth(0);
           }
         }
       }
       if (ykinfo.isYXTable(m_tablename)) {
         ykinfo.setYKSomeField(myTableModel);
         myTableModel.setYKInfo(ykinfo);
       }

       jtab.addTab(title1, imagetab, jscrolltab);
       jtab.setToolTipTextAt(tabindex, m_tablename);

       myTableModel.addTableModelListener(new javax.swing.event.
                                          TableModelListener() {
         public void tableChanged(TableModelEvent e) {
           //表内容修改后的操作、标志等
           int tabindex1 = jtab.getSelectedIndex();
           if (tabindex1 >= 0) {
             MyTableModel myTableModel1 = (MyTableModel) ( (MyJTable)
                 vListTable.elementAt(tabindex1)).getModel();
             if (myTableModel1.getCellChangedFlag()) {
               myTableModel1.setCellChangedFlag(false);
               jtab.setForegroundAt(tabindex1, Color.red);
             }
           } //if indextab>=0
         }
       });
       //处理ESCAPE、DELETE键
       listtable.addKeyListener(new java.awt.event.KeyAdapter() {
         public void keyPressed(KeyEvent e) {
           doPressKey(e);
         } //public void keyTyped(KeyEvent e) {
       });

     } //if(ralTabnoCount == 1)
     else { //有多个分区
       final JTabbedPane jtabbedPaneTabno = new JTabbedPane();
       vTabnoJTab.add(tabindex, jtabbedPaneTabno);
       jtabbedPaneTabno.setTabPlacement(JTabbedPane.BOTTOM);
       jtabbedPaneTabno.setBackground(new Color(ConstantDef.CRED,
                                                ConstantDef.CGREEN,
                                                ConstantDef.CBLUE));
       jscrolltab.getViewport().add(jtabbedPaneTabno, null); //增加分区的tab容器
       MyJTable firstlisttable = null;
       int colcount = 0; //总的列数

       int setpvalue = 10 / realTabnoCount;
       int count = 1;
       for (int m = 0; m < realTabnoCount; m++) {
         MyTableModel myTableModel = new MyTableModel((Object[][])
             vTabAreaData.elementAt(m * 2 + 1),
             (Object[]) vTabAreaData.elementAt(m * 2));

         myTableModel.setColType( (Vector) vColumnClass.elementAt(m));
         myTableModel.setLongValues( (Vector) vLongValues.elementAt(m));
         myTableModel.setTableType(tablesetinfo.tableFieldInfo.getTableType()); //vTableType.elementAt(tabindex).toString());
         myTableModel.setDBName(m_db, m_tablename);
         myTableModel.setKey(vKey);
         myTableModel.setListField(m_slistfield, m_vlistfield);
         myTableModel.init();
         myTableModel.initEnds();
         colcount += myTableModel.getColumnCount();
         MyJTable listtable = new MyJTable(myTableModel);

         if (m == 0) {
           vListTable.add(i, listtable);
           firstlisttable = listtable;
           m_managemodel.initFlag(myTableModel.getRowCount());
         }
         else {
           firstlisttable.addJTable(listtable);
         }

         myTableModel.setManageModel(m_managemodel);
         listtable.setFirstJTable(firstlisttable);
         JScrollPane jscrolltabnotab = new JScrollPane();

         listtable.setFontSize(fontsize);
         listtable.setTableSetInfo(tablesetinfo);
         listtable.setRealTable(m_realtable);
         listtable.setWhere(m_where);
         listtable.setPDBField(pdb_fieldtable);
         listtable.setCurTabno(m);
         listtable.setScrollTab(jscrolltabnotab);
         listtable.setNode(m_node);
         listtable.setRowHeight(rowheight);

         MyJTableHeader myjtableheader = new MyJTableHeader(listtable.
             getColumnModel());
         myjtableheader.setTable(listtable);
         myjtableheader.init();
         listtable.setTableHeader(myjtableheader);

         listtable.init();
         if (m_tablename.equals("遥信参数表") || m_tablename.equals("遥测参数表") ||
             m_tablename.equals("电度参数表") || m_tablename.equals("SCD设备参数表") ||
             m_tablename.equals("终端参数表")) {
           for (int t = 0; t < myTableModel.columnIdentifiers.size(); t++) {
             String code = myTableModel.columnIdentifiers.elementAt(t).
                 toString();
             int dfs = 0;
             if (code.equalsIgnoreCase("代码")) {
               //  tab=t;
               //  listtable.getColumnModel().getColumn(tab).setPreferredWidth(0);
             }
           }
         }

         if (ykinfo.isYXTable(m_tablename)) {
           ykinfo.setYKSomeField(myTableModel);
         }

         jtabbedPaneTabno.add(m_vTabnoName.elementAt(m).toString(),
                              jscrolltabnotab);
         jscrolltabnotab.getViewport().add(listtable, null);

         listtable.addKeyListener(new java.awt.event.KeyAdapter() {
           public void keyPressed(KeyEvent e) {
             doPressKey(e);
           } //public void keyTyped(KeyEvent e) {
         });
         myTableModel.addTableModelListener(new javax.swing.event.
                                            TableModelListener() {
           public void tableChanged(TableModelEvent e) {

             //表内容修改后的操作、标志等
             int tabindex1 = jtab.getSelectedIndex();
             int tabindex2 = jtabbedPaneTabno.getSelectedIndex();
             MyJTable myjtable = null;
             MyTableModel myTableModel1 = null;
             if (tabindex1 >= 0 && tabindex2 >= 0) {
               myjtable = (MyJTable) vListTable.elementAt(tabindex1);
               if (tabindex2 == 0) {
                 myTableModel1 = (MyTableModel) myjtable.getModel();
               }
               else {
                 MyJTable jtabtmp = myjtable.getJTable(tabindex2 - 1);
                 myTableModel1 = (MyTableModel) jtabtmp.getModel();
               }
               if (myTableModel1.getCellChangedFlag()) {
                 myTableModel1.setCellChangedFlag(false);
                 jtab.setForegroundAt(tabindex1, Color.red);
               }
             } //if indextab>=0
           }
         });
       } //for m
       if (ykinfo.isYXTable(m_tablename)) {
         MyTableModel mymodelyk = (MyTableModel) firstlisttable.getModel();
         mymodelyk.setYKInfo(ykinfo);
         for (int k = 0; k < realTabnoCount - 1; k++) {
           mymodelyk = (MyTableModel) ( (MyJTable) firstlisttable.getJTable(k)).
               getModel();
           mymodelyk.setYKInfo(ykinfo);
         }
       }
       firstlisttable.setColCount(colcount);
       jtab.addTab(title1, imagetab, jscrolltab);
       jtab.setToolTipTextAt(tabindex, m_tablename);

       jtabbedPaneTabno.addKeyListener(new java.awt.event.KeyAdapter() {
         public void keyPressed(KeyEvent e) {
           doPressKey(e);
         } //public void keyPressed(KeyEvent e) {
       });
     } //else 有多个分区
   } //if i
   if (jtab.getTabCount() > tabindex) {
     jtab.setSelectedIndex(tabindex);
   }
  }
//-------end
//end
  //用户点击树节点后需要打开显示表
  public void listTableInfo() {
    if (m_tablename == null || m_tablename == "") {
      return;
    }
    int tabcount = jtab.getTabCount();
    int i = 0;
    int tabindex = 9999;
    int level = m_node.getLevel();
    int tab = 0;
    String title1 = "";
    title1 = m_node.toString().trim();
    DefaultMutableTreeNode node1 = m_node;
    if(m_node.getLevel()==7){
      for (i = 6; i < level; i++) {
        node1 = (DefaultMutableTreeNode) node1.getParent();
        title1 = node1.toString().trim() + "/" + title1;
      }

    }
    else{
      for (i = 2; i < level; i++) {
        node1 = (DefaultMutableTreeNode) node1.getParent();
        title1 = node1.toString().trim() + "/" + title1;
      }
    }
    for (i = 0; i < tabcount; i++) {
      String tabtitle = jtab.getTitleAt(i).trim(); //用全路径
      if (tabtitle.equalsIgnoreCase(title1) == true) {
        tabindex = i;
        break;
      }
    }
    if (i >= tabcount) { //当前显示中没有该表

      if (m_tabletabcount > 5) {
        m_tabletabcount = 5; //分区最多5个
      }
      if (m_tabletabcount <= 0) {
        m_tabletabcount = 1; //分区数至少为1
      }
      tabindex = i;
      vKey.removeAllElements(); //要清除，否则以后就得不到正确的结果
      vTabAreaData.removeAllElements(); //清除前一表的所有数据
      vColumnClass.removeAllElements();

      m_managemodel = new ManageModel();
      //添加条件筛选  hongxiang
      if(flwhere!=null && flwhere!=""){
        if (m_where == null || m_where == "") {
           m_where = flwhere;
        }
        else
          m_where = m_where + " and " + flwhere;
      }
      //end
      //获得表内容

      int fieldnums = getTableContent(tabindex);

      if (fieldnums < 1) {
        return;
      }
      //------hongxiang
      String title2 = "";
      title2 = title2 + m_node.toString().trim();
      DefaultMutableTreeNode node2 = m_node;
      for (int t = 0; t < level; t++) {
        node2 = (DefaultMutableTreeNode) node2.getParent();
        title2 = node2.toString().trim() + "/" + title2;
      }
      if(vSelectFlag){
        title2 = "     树节点路径:/" + title2 + " [筛选条件 " +
                flwhere + "]";
      }else
         title2 = "     树节点路径:/" + title2;
      vTitle.addElement(title2);

//--------------
//vTitle

      vTableTabnoCount.add(tabindex, Integer.toString(m_tabletabcount)); //保存每个表的分区数
      JScrollPane jscrolltab = new JScrollPane();
      jscrolltab.setBackground(new Color(ConstantDef.CRED, ConstantDef.CGREEN,
                                         ConstantDef.CBLUE));
      int realTabnoCount = Integer.parseInt(vTableTabnoCount.elementAt(
          tabindex).toString().trim()); //实际分区数
      YKInfo ykinfo = new YKInfo();

      vOpenLeafNode.add(m_node);

      m_curtablename = title1;
      if (realTabnoCount == 1) { //实际分区数为1

        vTabnoJTab.add(tabindex, Integer.toString(0));
        MyTableModel myTableModel = new MyTableModel( (Object[][]) vTabAreaData.
            elementAt(1), (Object[]) vTabAreaData.elementAt(0)); //只有1个分区的数据分别放在0,1两个位置

        m_managemodel.initFlag(myTableModel.getRowCount());

        myTableModel.setManageModel(m_managemodel);

        myTableModel.setTableType(tablesetinfo.tableFieldInfo.getTableType());

        myTableModel.setDBName(m_db, m_tablename);

        myTableModel.setKey(vKey);

        myTableModel.setColType( (Vector) vColumnClass.elementAt(0));
        myTableModel.setLongValues( (Vector) vLongValues.elementAt(0));
        myTableModel.setListField(m_slistfield, m_vlistfield);
        myTableModel.init();
        myTableModel.initEnds();

        MyJTable listtable = new MyJTable(myTableModel);
        vListTable.add(i, listtable);

        jscrolltab.getViewport().add(listtable, null);

        listtable.setColCount(myTableModel.getColumnCount());
        listtable.setFontSize(fontsize);
        listtable.setTableSetInfo(tablesetinfo);
        listtable.setRealTable(m_realtable);
        listtable.setWhere(m_where);
        listtable.setPDBField(pdb_fieldtable);
        listtable.setScrollTab(jscrolltab);
        listtable.setNode(m_node);
        listtable.setRowHeight(rowheight);
        listtable.setCurTabno(0);
        listtable.setFirstJTable(listtable);

        MyJTableHeader myjtableheader = new MyJTableHeader(listtable.
            getColumnModel());
        myjtableheader.setTable(listtable);
        myjtableheader.init();
        listtable.setTableHeader(myjtableheader);
        listtable.init();
        if (m_tablename.equals("遥信参数表") || m_tablename.equals("遥测参数表") ||
            m_tablename.equals("电度参数表") || m_tablename.equals("SCD设备参数表") ||
            m_tablename.equals("终端参数表")) {
          for (int t = 0; t < fieldnums; t++) {
            String code = myTableModel.columnIdentifiers.elementAt(t).toString();
            if (code.equalsIgnoreCase("代码")) {
              //  tab=t;
              //   listtable.getColumnModel().getColumn(tab).setPreferredWidth(0);
            }
          }
        }
        if (ykinfo.isYXTable(m_tablename)) {
          ykinfo.setYKSomeField(myTableModel);
          myTableModel.setYKInfo(ykinfo);
        }

        jtab.addTab(title1, imagetab, jscrolltab);
        jtab.setToolTipTextAt(tabindex, m_tablename);

        myTableModel.addTableModelListener(new javax.swing.event.
                                           TableModelListener() {
          public void tableChanged(TableModelEvent e) {
            //表内容修改后的操作、标志等
            int tabindex1 = jtab.getSelectedIndex();
            if (tabindex1 >= 0) {
              MyTableModel myTableModel1 = (MyTableModel) ( (MyJTable)
                  vListTable.elementAt(tabindex1)).getModel();
              if (myTableModel1.getCellChangedFlag()) {
                myTableModel1.setCellChangedFlag(false);
                jtab.setForegroundAt(tabindex1, Color.red);
              }
            } //if indextab>=0
          }
        });
        //处理ESCAPE、DELETE键
        listtable.addKeyListener(new java.awt.event.KeyAdapter() {
          public void keyPressed(KeyEvent e) {
            doPressKey(e);
          } //public void keyTyped(KeyEvent e) {
        });

      } //if(ralTabnoCount == 1)
      else { //有多个分区
        final JTabbedPane jtabbedPaneTabno = new JTabbedPane();
        vTabnoJTab.add(tabindex, jtabbedPaneTabno);
        jtabbedPaneTabno.setTabPlacement(JTabbedPane.BOTTOM);
        jtabbedPaneTabno.setBackground(new Color(ConstantDef.CRED,
                                                 ConstantDef.CGREEN,
                                                 ConstantDef.CBLUE));
        jscrolltab.getViewport().add(jtabbedPaneTabno, null); //增加分区的tab容器
        MyJTable firstlisttable = null;
        int colcount = 0; //总的列数

        int setpvalue = 10 / realTabnoCount;
        int count = 1;
        for (int m = 0; m < realTabnoCount; m++) {
          MyTableModel myTableModel = new MyTableModel((Object[][])
              vTabAreaData.elementAt(m * 2 + 1),
              (Object[]) vTabAreaData.elementAt(m * 2));

          myTableModel.setColType( (Vector) vColumnClass.elementAt(m));
          myTableModel.setLongValues( (Vector) vLongValues.elementAt(m));
          myTableModel.setTableType(tablesetinfo.tableFieldInfo.getTableType()); //vTableType.elementAt(tabindex).toString());
          myTableModel.setDBName(m_db, m_tablename);
          myTableModel.setKey(vKey);
          myTableModel.setListField(m_slistfield, m_vlistfield);
          myTableModel.init();
          myTableModel.initEnds();
          colcount += myTableModel.getColumnCount();
          MyJTable listtable = new MyJTable(myTableModel);

          if (m == 0) {
            vListTable.add(i, listtable);
            firstlisttable = listtable;
            m_managemodel.initFlag(myTableModel.getRowCount());
          }
          else {
            firstlisttable.addJTable(listtable);
          }

          myTableModel.setManageModel(m_managemodel);
          listtable.setFirstJTable(firstlisttable);
          JScrollPane jscrolltabnotab = new JScrollPane();

          listtable.setFontSize(fontsize);
          listtable.setTableSetInfo(tablesetinfo);
          listtable.setRealTable(m_realtable);
          listtable.setWhere(m_where);
          listtable.setPDBField(pdb_fieldtable);
          listtable.setCurTabno(m);
          listtable.setScrollTab(jscrolltabnotab);
          listtable.setNode(m_node);
          listtable.setRowHeight(rowheight);

          MyJTableHeader myjtableheader = new MyJTableHeader(listtable.
              getColumnModel());
          myjtableheader.setTable(listtable);
          myjtableheader.init();
          listtable.setTableHeader(myjtableheader);

          listtable.init();
          if (m_tablename.equals("遥信参数表") || m_tablename.equals("遥测参数表") ||
              m_tablename.equals("电度参数表") || m_tablename.equals("SCD设备参数表") ||
              m_tablename.equals("终端参数表")) {
            for (int t = 0; t < myTableModel.columnIdentifiers.size(); t++) {
              String code = myTableModel.columnIdentifiers.elementAt(t).
                  toString();
              int dfs = 0;
              if (code.equalsIgnoreCase("代码")) {
                //  tab=t;
                //  listtable.getColumnModel().getColumn(tab).setPreferredWidth(0);
              }
            }
          }

          if (ykinfo.isYXTable(m_tablename)) {
            ykinfo.setYKSomeField(myTableModel);
          }

          jtabbedPaneTabno.add(m_vTabnoName.elementAt(m).toString(),
                               jscrolltabnotab);
          jscrolltabnotab.getViewport().add(listtable, null);

          listtable.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(KeyEvent e) {
              doPressKey(e);
            } //public void keyTyped(KeyEvent e) {
          });
          myTableModel.addTableModelListener(new javax.swing.event.
                                             TableModelListener() {
            public void tableChanged(TableModelEvent e) {

              //表内容修改后的操作、标志等
              int tabindex1 = jtab.getSelectedIndex();
              int tabindex2 = jtabbedPaneTabno.getSelectedIndex();


              MyJTable myjtable = null;
              MyTableModel myTableModel1 = null;
              if (tabindex1 >= 0 && tabindex2 >= 0) {
                myjtable = (MyJTable) vListTable.elementAt(tabindex1);
                if (tabindex2 == 0) {
                  myTableModel1 = (MyTableModel) myjtable.getModel();
                }
                else {
                  MyJTable jtabtmp = myjtable.getJTable(tabindex2 - 1);
                  myTableModel1 = (MyTableModel) jtabtmp.getModel();
                }
                if (myTableModel1.getCellChangedFlag()) {
                  myTableModel1.setCellChangedFlag(false);
                  jtab.setForegroundAt(tabindex1, Color.red);
                }
              } //if indextab>=0
            }
          });
        } //for m
        if (ykinfo.isYXTable(m_tablename)) {
          MyTableModel mymodelyk = (MyTableModel) firstlisttable.getModel();
          mymodelyk.setYKInfo(ykinfo);
          for (int k = 0; k < realTabnoCount - 1; k++) {
            mymodelyk = (MyTableModel) ( (MyJTable) firstlisttable.getJTable(k)).
                getModel();
            mymodelyk.setYKInfo(ykinfo);
          }
        }
        firstlisttable.setColCount(colcount);
        jtab.addTab(title1, imagetab, jscrolltab);
        jtab.setToolTipTextAt(tabindex, m_tablename);

        jtabbedPaneTabno.addKeyListener(new java.awt.event.KeyAdapter() {
          public void keyPressed(KeyEvent e) {
            doPressKey(e);
          } //public void keyPressed(KeyEvent e) {
        });
      } //else 有多个分区
    } //if i
    if (jtab.getTabCount() > tabindex) {
      jtab.setSelectedIndex(tabindex);
    }
  }

  //处理按键事件
  public void doPressKey(KeyEvent e) {
    ConstantDef.setLoginTime();
    User user = ConstantDef.getUserInstance();
    int tabcount = jtab.getTabCount();
    int tabindex1 = jtab.getSelectedIndex();

    int presskey = e.getKeyCode();
    if (presskey == KeyEvent.VK_ESCAPE || presskey == KeyEvent.VK_DELETE ||
        presskey == e.VK_X || presskey == e.VK_C || presskey == e.VK_S ||
        presskey == e.VK_D) {


      if (tabcount > 0 && tabindex1 >= 0) {
        if (presskey == KeyEvent.VK_ESCAPE) {
          int tabnocount = Integer.parseInt(vTableTabnoCount.elementAt(
              tabindex1).toString()); //表的分区数
          MyJTable jtable1;
          if (presskey == KeyEvent.VK_ESCAPE) {
            jtable1 = (MyJTable) vListTable.elementAt(tabindex1);
            jtable1.clearSelection();

            if (tabnocount > 1) {
              for (int k = 0; k < tabnocount - 1; k++) {
                MyJTable jtabtmp = jtable1.getJTable(k);
                jtabtmp.clearSelection();
              }
            }
          }
        }
        else if (presskey == e.VK_DELETE ||
                 (presskey == e.VK_D && e.isControlDown())) {
          int removerownums = removeRow(tabindex1);
          if (removerownums > 0) {
            jtab.setForegroundAt(tabindex1, Color.red);
          }
        }
        else if (presskey == e.VK_S && e.isControlDown()) { //ctrl+S = save
          if (user.getEditPara() == 0) {
            return;
          }
          int saveflag = saveChange(tabindex1,
                                    jtab.getToolTipTextAt(tabindex1).
                                    trim());
          if (saveflag > 0) {
            jtab.setForegroundAt(tabindex1, Color.black);
          }
        }
        else if (presskey == e.VK_C && e.isControlDown()) { //ctrl+C = insert
          if (user.getEditPara() == 0) {
            return;
          }
       //   JOptionPane.showMessageDialog(null, "为什么执行这ctrl +c呢", "错误",
       //                                 JOptionPane.WARNING_MESSAGE);

          int ret = insertRow(tabindex1);
          if (ret > 0) {
            jtab.setForegroundAt(tabindex1, Color.red);
          }

        }
        else if (presskey == e.VK_X && e.isControlDown()) { //ctrl+Z = add
          if (user.getEditPara() == 0) {
            return;
          }
          int ret = addRow(tabindex1);
          if (ret > 0) {
            jtab.setForegroundAt(tabindex1, Color.red);
          }
        }
      } //if(tabcount>0 && tabindex1>=0)
    } //if(e.getKeyChar()   == KeyEvent.VK_ESCAPE || e.getKeyChar() == KeyEvent.VK_DELETE)
    //------处理tab键程序会死掉的问题 hx
    /*
    else if(presskey ==e.VK_SHIFT || presskey==e.VK_CONTROL){
      return;
     }
     else{
       e.setKeyCode(0);
     }
     */
  }

//返回表字段的个数，注意是剔除了隐含字段后的个数
  public int getTableContent(int tabno) {
    int fieldnums = -1;
    if (m_tablename == null || m_tablename == "") {
      return fieldnums;
    }
    String fieldname = "";
    String orderfn = m_orderfn;
    int tabletabcount = m_tabletabcount;

    tablesetinfo = new TableSetInfo();
    tablesetinfo.setTabCount(tabletabcount);

    tablesetinfo.tableFieldInfo = new FieldProp(m_db.trim(), m_tablename.trim());
    tablesetinfo.tableFieldInfo.getTableInfo(); //得到表的域信息
    tablesetinfo.tableFieldInfo.getPDBProp();   //得到表在pdb_field中信息
    fieldnums = tablesetinfo.tableFieldInfo.fieldnums;
    vTabAreaData.removeAllElements();
    vLongValues.setSize(0);

    Object[] columnNames; //列名
    Object[][] data; //表数据

    int[] iTabAreaNo = new int[tabletabcount]; //保存每区的列数
    int[] icurdataptr = new int[tabletabcount]; //为使data保存数据时，是按照区号分开的，每区当前的列号指针
    //如代码列，在表中可能为第3列，但放在分区2中后，加上关键字段列后就有可能是第2列

    for (int v = 0; v < tabletabcount; v++) {
      iTabAreaNo[v] = 0; //每一区的域数初始化为0
      icurdataptr[v] = 0;
    }

    //找出主键列的域名
    for (int kk = 0; kk < fieldnums; kk++) {
      if (Integer.parseInt(tablesetinfo.tableFieldInfo.getATableInfo(
          kk, ConstantDef.FIELD_KEY)) == 1) {
        vKey.addElement(tablesetinfo.tableFieldInfo.getATableInfo(
            kk, ConstantDef.FIELD_NAME));
      }
    } //for k

    int recNumsInPDBF = tablesetinfo.tableFieldInfo.buf.length /
        pdb_fieldtable.fieldnums; //tablename在pdb_field中的记录数
    int tmpcount = 0;

    //找列所在的区号
    for (int k1 = 0; k1 < recNumsInPDBF; k1++) {
      //pdb_field设置的每个域都存在。否则会导致分区时每区的数目设置不对，导致出错。
      String fieldnametmp = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                        pdb_fieldtable.pdb_field_describle_no +
                                        k1 * pdb_fieldtable.fieldnums]);
      if (tablesetinfo.tableFieldInfo.getColumnNo(fieldnametmp) == -1) { ////jhongtu ++ 20060117
        JOptionPane.showMessageDialog(null,
                                      "pdb_field中设置的域不存在",
                                      "警告",
                                      JOptionPane.ERROR_MESSAGE);
        return -1;
        //continue;jhongtu -- 20060117
      }
      int status = Integer.parseInt(pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                                pdb_fieldtable.
                                                pdb_field_status_no +
                                                k1 * pdb_fieldtable.fieldnums]));
      if (status == 0) { //不显示
        m_managemodel.addAField(fieldnametmp);
        continue;
      }
      int coltabno = Integer.parseInt(pdll.substr(tablesetinfo.tableFieldInfo.
                                                  buf[pdb_fieldtable.
                                                  pdb_field_tabno_no +
                                                  k1 * pdb_fieldtable.fieldnums])); //在pdb_field中本表的每列所在的区号
      //解决极端情况
      if (coltabno > tabletabcount) {
        coltabno = tabletabcount; //区号设置有矛盾时，以tabletabcount为准
      }
      if (coltabno <= 0) {
        coltabno = 1; //区号从1开始
      }
      iTabAreaNo[coltabno - 1] += 1; //得到每区的列数
      tablesetinfo.tableFieldInfo.buf[pdb_fieldtable.pdb_field_tabno_no + k1 *
          pdb_fieldtable.fieldnums] = Integer.toString(coltabno).getBytes(); //重新调整区号
      if (coltabno > 1) {
        tmpcount++; //由于缺省的都放在1区，所以要找出其余区的总和，方便分配内存
      }
    }

    if (fieldnums > 0) {
      int listfieldid = -1;
      m_vlistfield.removeAllElements();

      Vector vFieldname = new Vector(0);
      int showfieldcount = 0;
      for (int i = 0; i < fieldnums; i++) {
        String fdname = tablesetinfo.tableFieldInfo.getATableInfo(i,
            ConstantDef.FIELD_NAME);
        if (!m_managemodel.isHideFiled(fdname)) {
          if (showfieldcount > 0) {
            fieldname += ","; //组建查询字段
          }
          fieldname += fdname;
          vFieldname.add(fdname);
          if (m_slistfield != "" && m_slistfield.equalsIgnoreCase(fdname)) {
            listfieldid = showfieldcount;
          }
          showfieldcount++;
        }
      }

      fieldnums = showfieldcount; ///隐含的列剔除了

      if (fieldnums <= 0)
        return fieldnums;

      int[] fieldcount = new int[1];
      byte[][] buf = new byte[0][0];
      if (!m_where.equalsIgnoreCase("") &&
          (m_tablename.equalsIgnoreCase(ConstantDef.KGBHGXPARA) ||
           m_tablename.equalsIgnoreCase(ConstantDef.KGBHMKGXPARA))) {
        String newwh = "开关遥信代码 in (select 代码 from 开关量视图  where " + m_where +
            ")"; //保护信号量视图,开关量视图
        buf = pdll.queryAnyTable(m_db, m_tablename, fieldname, newwh, orderfn,
                                 fieldcount, false);
      }
      else {
        buf = pdll.queryAnyTable(m_db, m_tablename, fieldname, m_where, orderfn,
                                 fieldcount, false);
      }
      int valuecount = buf.length;

      int rox = valuecount / fieldnums; //计算得出行数

      columnNames = new String[fieldnums - tmpcount]; //1区的列数
      data = new Object[rox][columnNames.length];
      vTabAreaData.add(0, columnNames); //因为第1区有很多缺省的和没有设置的，特殊处理
      vTabAreaData.add(1, data);

      Vector vcolclass = new Vector(0);
      for (int k1 = 0; k1 < columnNames.length; k1++) {
        vcolclass.add("0");
      }
      vColumnClass.add(0, vcolclass);
      vLongValues.add(0, vcolclass.clone());

      for (int k1 = 1; k1 < tabletabcount; k1++) {
        columnNames = new String[iTabAreaNo[k1]]; //每区的列数
        data = new Object[rox][columnNames.length];
        vTabAreaData.add(k1 * 2, columnNames); //保存每一列的列名分配空间
        vTabAreaData.add(k1 * 2 + 1, data); //保存每一行的数据分配空间
        vcolclass = new Vector(0);
        for (int k2 = 0; k2 < columnNames.length; k2++) {
          vcolclass.add("0");
        }
        vLongValues.add(k1, vcolclass.clone()); //为了初始化，没有其他意义.分配每区的列数大小
        vColumnClass.add(k1, vcolclass);
      }

      columnNames = new String[fieldnums];
      data = new Object[rox][columnNames.length];

      NumberFormat nf = NumberFormat.getInstance(Locale.CHINA);
      if (nf instanceof DecimalFormat) {
        ( (DecimalFormat) nf).setDecimalSeparatorAlwaysShown(false);
      }
      nf.setParseIntegerOnly(false);

      int dotnum = 0; //小数的位数

      for (int i = 0; i < fieldnums; i++) {
        //获得表列的名称
        String tmpColumnName = vFieldname.elementAt(i).toString(); //new String(pdll.substr(tablesetinfo.tableFieldInfo.tableinfo[i * 3]));
        columnNames[i] = tmpColumnName; //列名

        dotnum = 0; //缺省的小数尾数2位

        //找到域在pdb_field中的设置的输入方式
        //比较每一列的名称，找到pdb_field中对该列的设置，后根据相应的设置来显示该列
        int m = 0;
        int evaluemode = -1; //列的赋值方式
        int curColTabArea = 1; //当前列的区号
        for (m = 0; m < recNumsInPDBF; m++) {
          String fieldname2 = pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                          pdb_fieldtable.
                                          pdb_field_describle_no +
                                          m * pdb_fieldtable.fieldnums]);
          if (fieldname2.equalsIgnoreCase(tmpColumnName)) {
            curColTabArea = Integer.parseInt(pdll.substr(tablesetinfo. //当前列的区号
                tableFieldInfo.buf[
                pdb_fieldtable.pdb_field_tabno_no +
                m * pdb_fieldtable.fieldnums])); //在上面已做了调整，不须考虑极端情况
            break; //在pdb_field中找到了这个域的设置
          }
        } //for m

        int colno = tablesetinfo.tableFieldInfo.getColumnNo(tmpColumnName);
        if (colno == -1) { //jhongtu ++ 20060122 begin
          JOptionPane.showMessageDialog(null,
                                        "数据库表域名分析出错!",
                                        "警告",
                                        JOptionPane.ERROR_MESSAGE);
          return -1;
        } //end
        String type = pdll.substr(tablesetinfo.tableFieldInfo.tableinfo[colno *
                                  3 + 1]);
        int itype = Integer.parseInt(type);

        if (m < recNumsInPDBF) { //在pdb_field中找到了这个域的设置,判断设置的内容
          evaluemode = Integer.parseInt(pdll.substr(tablesetinfo.tableFieldInfo.
              buf[pdb_fieldtable.pdb_field_evaluemode_no +
              m * pdb_fieldtable.fieldnums])); //赋值方式
          if (evaluemode == ConstantDef.IN_BYKEYBOARD &&
              (itype == ConstantDef.CS_FLOAT_TYPE ||
               itype == ConstantDef.CS_REAL_TYPE)) {
            int dotnumf = Integer.parseInt(pdll.substr(tablesetinfo.
                tableFieldInfo.buf[
                pdb_fieldtable.pdb_field_dotnum_no +
                m * pdb_fieldtable.fieldnums]));
            if (dotnumf > 1) { //只有在要求小数点三位以上的才这样做
              evaluemode = ConstantDef.IN_FLOAT;
            }
          }

          tablesetinfo.setInputMode(curColTabArea - 1,
                                    icurdataptr[curColTabArea - 1], evaluemode);

          dotnum = Integer.parseInt(pdll.substr(tablesetinfo.tableFieldInfo.buf[
                                                pdb_fieldtable.
                                                pdb_field_dotnum_no +
                                                m * pdb_fieldtable.fieldnums])); //小数尾数
          if (dotnum > 0) {
            nf.setMinimumFractionDigits(dotnum);
            nf.setMaximumFractionDigits(dotnum);
          }
        } //if m

        //有rox==0的情况,此时MyTableModel的getColumnClass需要列的类型
        switch (itype) {
          case ConstantDef.CS_CHAR_TYPE:
          case ConstantDef.CS_VARCHAR_TYPE: {
            ( (Vector) vColumnClass.elementAt(curColTabArea - 1)).set(
                icurdataptr[curColTabArea - 1],
                Integer.toString(ConstantDef.STRINGTYPE));
            break;
          }
          case ConstantDef.CS_TINYINT_TYPE:
          case ConstantDef.CS_SMALLINT_TYPE:
          case ConstantDef.CS_USHORT_TYPE:
          case ConstantDef.CS_INT_TYPE:
          case ConstantDef.CS_LONG_TYPE: {
            if (evaluemode == ConstantDef.IN_CHECKBOX) { //check box
              ( (Vector) vColumnClass.elementAt(curColTabArea - 1)).set(
                  icurdataptr[curColTabArea - 1],
                  Integer.toString(ConstantDef.BOOLEAN));
            }
            else {
              ( (Vector) vColumnClass.elementAt(curColTabArea - 1)).set(
                  icurdataptr[curColTabArea - 1],
                  Integer.toString(ConstantDef.DATATYPE));
            }
            break;
          }
          case ConstantDef.CS_FLOAT_TYPE:
          case ConstantDef.CS_REAL_TYPE: {
            ( (Vector) vColumnClass.elementAt(curColTabArea - 1)).set(
                icurdataptr[curColTabArea - 1],
                Integer.toString(ConstantDef.FLOATTYPE));
            break;
          } //case
          default: {
            ( (Vector) vColumnClass.elementAt(curColTabArea - 1)).set(
                icurdataptr[curColTabArea - 1],
                Integer.toString(ConstantDef.STRINGTYPE));
          }
        } //switch

        Vector tmplongvalue = (Vector) vLongValues.elementAt(curColTabArea - 1);
        String tmp = "";
        for (int j = 0; j < rox; j++) {
          tmp = pdll.substr(buf[j * fieldnums + i]); //j行i列的值
          if (i == listfieldid) {
            m_vlistfield.add(tmp);
          }
          switch (itype) {
            case ConstantDef.CS_CHAR_TYPE:
            case ConstantDef.CS_VARCHAR_TYPE: {
              data[j][i] = new String(tmp);
              break;
            }
            case ConstantDef.CS_TINYINT_TYPE:
            case ConstantDef.CS_SMALLINT_TYPE: {
              if (evaluemode == ConstantDef.IN_CHECKBOX) { //check box
                if (tmp.equalsIgnoreCase("0")) {
                  data[j][i] = new Boolean(false);
                }
                else {
                  data[j][i] = new Boolean(true);
                }
              } //if evaluemode
              else {
                data[j][i] = new Short(tmp);
              }
              break;
            }
            case ConstantDef.CS_USHORT_TYPE:
            case ConstantDef.CS_INT_TYPE: {
              if (evaluemode == ConstantDef.IN_CHECKBOX) { //check box
                if (tmp.equalsIgnoreCase("0")) {
                  data[j][i] = new Boolean(false);
                }
                else {
                  data[j][i] = new Boolean(true);
                }
              } //if evaluemode
              else {
                data[j][i] = new Integer(tmp);
              }
              break;
            }
            case ConstantDef.CS_LONG_TYPE: {
              if (evaluemode == ConstantDef.IN_CHECKBOX) { //check box
                if (tmp.equalsIgnoreCase("0")) {
                  data[j][i] = new Boolean(false);
                }
                else {
                  data[j][i] = new Boolean(true);
                }
              } //if evaluemode
              else {
                data[j][i] = new Long(tmp);
              }
              break;
            }
            case ConstantDef.CS_FLOAT_TYPE:
            case ConstantDef.CS_REAL_TYPE: {
              if (dotnum > 0) {
                tmp = nf.format(new Float(tmp));
                int index = tmp.indexOf(",");
                while (index > 0) {
                  String preStr = tmp.substring(0, index);
                  tmp = tmp.substring(index + 1);
                  tmp = preStr + tmp;
                  index = tmp.indexOf(",");
                }
              }
              data[j][i] = new Float(tmp); //new Float(Float.valueOf(tmp).floatValue);
              break;
            }
          } //switch
          int curColumnNo = icurdataptr[curColTabArea - 1];
          ( (Object[][]) vTabAreaData.elementAt( (curColTabArea - 1) * 2 + 1))[
              j][curColumnNo] = data[j][i]; //data
          if (tmp.length() >
              tmplongvalue.elementAt(curColumnNo).toString().length()) {
            tmplongvalue.setElementAt(tmp, curColumnNo); //
          }
        } //for j

        if (PreProcessor.ALIAS) {
          String alias=ApplicationDB.frameDB.GetAliasFromFieldName(m_tablename,tmpColumnName);
          if(alias.length()>0)
            ((Object[]) vTabAreaData.elementAt((curColTabArea - 1) * 2))[
                 icurdataptr[curColTabArea - 1]] = new String(alias); //列名
          else
             ((Object[]) vTabAreaData.elementAt( (curColTabArea - 1) * 2))[
                 icurdataptr[curColTabArea - 1]] = new String(tmpColumnName.trim()); //列名
        }
        else

          ( (Object[]) vTabAreaData.elementAt( (curColTabArea - 1) * 2))[
              icurdataptr[curColTabArea - 1]] = new String(tmpColumnName.trim()); //列名
        icurdataptr[curColTabArea - 1]++;
      } //for i
    } //if

    return fieldnums; //如果得到表的数据失败，或表不存在，就不再在表格中显示;此时fieldnums==-1;
  } //public



  public String getModelDB(String appTypeDescribe) {
    int size = serviceTypeTable.length; //vModelDBName.size() ;
    String db = ConstantDef.MODELDB;
    try {
      if (size == 0) {
        return db;
      }
      for (int i = 0; i < size; i++) {
        if (serviceTypeTable[i].serviceDescrible.equalsIgnoreCase(
            appTypeDescribe)) {
          return serviceTypeTable[i].ParaTablename;
        }
      } //for i
    }
    catch (NullPointerException ne) {
      return ConstantDef.MODELDB;
    }
    return db;
  } ///public
  //----CIM模型必须 by hx
  //----通过描述得到类型
 public String getModelDBType(String appTypeDescribe) {
   int size = serviceTypeTable.length; //vModelDBName.size() ;
   String db = ConstantDef.MODELDB;
   try {
     if (size == 0) {
       return db;
     }
     for (int i = 0; i < size; i++) {
       if (serviceTypeTable[i].serviceDescrible.equalsIgnoreCase(
           appTypeDescribe)) {
         return serviceTypeTable[i].serviceType;
       }
     } //for i
   }
   catch (NullPointerException ne) {
     return ConstantDef.MODELDB;
   }
   return db;
 } ///public

 //----通过描述得到类型

 public String getParaSTATION(String STATIONDescribe) {
   ///-------------------
   byte[][] buf;
   int[] fieldcount = new int[1];
   buf = pdll.queryAnyTable(ConstantDef.MODELDB, "站所线系参数表",
                            "代码,描述", "", "", fieldcount, false);
   int len = buf.length;
   try {
     if (len > 0) {
       for (int i = 0; i < len; i++) {
         if (i % fieldcount[0] == 0) {
           if (pdll.substr(buf[i + 1]).equalsIgnoreCase(STATIONDescribe)) {
             return pdll.substr(buf[i]);
           }
         } //for
       } //if(len >0)

     } ///public
   }
   catch (NullPointerException ne) {
     return ConstantDef.MODELDB;
   }
   return STATIONDescribe;

 }

public String getParaRTU(String RtuDescribe) {
   ///-------------------
   byte[][] buf;
   int[] fieldcount = new int[1];
   buf = pdll.queryAnyTable(ConstantDef.MODELDB, "终端参数表",
                            "终端序号,描述", "", "", fieldcount, false);
   int len = buf.length;
   try {
     if (len > 0) {
       for (int i = 0; i < len; i++) {
         if (i % fieldcount[0] == 0) {
           if (pdll.substr(buf[i + 1]).equalsIgnoreCase(RtuDescribe)) {
             return pdll.substr(buf[i]);
           }
         } //for
       } //if(len >0)

     } ///public
   }
   catch (NullPointerException ne) {
     return ConstantDef.MODELDB;
   }
   return RtuDescribe;

 }

  //----CIM模型必须
  public int addRow(int tabno) {
    return addRowsInsertRows(tabno, ConstantDef.DB_RECORD_ADD);
  }

  public int removeRow(int tabno) {
    if (ConstantDef.getUserInstance().getEditPara() == 0) {
      return -1;
    }
    int curTabnoCount = Integer.parseInt(vTableTabnoCount.elementAt(
        tabno).toString().trim()); //表的分区数
    MyJTable jtable1 = (MyJTable) vListTable.elementAt(tabno);
    MyTableModel myTableModel1 = (MyTableModel) jtable1.getModel();

    if (PreProcessor.TableLock) { //表互斥修改  ---洪祥，判断表是否在修改
      byte[][] buf;
      int[] fieldcount = new int[1];
      String wh;
      wh = "tablename = '" + myTableModel1.getTableName() + "'";
      buf = pdll.queryAnyTable(ConstantDef.MODELDB, "table_status",
                               "tablename", wh, "", fieldcount, false);
      int len = buf.length;
      if (myTableModel1.getSaveFlag() == false) {
        if (len > 0) {
          JOptionPane.showMessageDialog(null, "此表正在编辑中", "提示",
                                        JOptionPane.WARNING_MESSAGE);
          return -1;
        }
      }
      //表互斥修改
    }
    if (!jtable1.getTableType().equalsIgnoreCase("U")) {
      JOptionPane.showMessageDialog(this, "不是基本表，不能编辑!");
      return -1;
    }
    if (myTableModel1.getKeyCount() < 1) {
      JOptionPane.showMessageDialog(this, "表没有关键字段，不能编辑!");
      return -1;
    }
    int[] rowselects = new int[0];
    if (curTabnoCount == 1) {
      rowselects = jtable1.getSelectedRows();
    }
    else {
      JTabbedPane tabnoJTab = (JTabbedPane) vTabnoJTab.elementAt(tabno);
      int tabno2 = tabnoJTab.getSelectedIndex();
      if (tabno2 >= 0 && tabno2 < curTabnoCount) {
        if (tabno2 == 0) {
          rowselects = jtable1.getSelectedRows();
        }
        else {
          MyJTable myjtabtmp = jtable1.getJTable(tabno2 - 1);
          rowselects = myjtabtmp.getSelectedRows();
        }
      }
    }
    for (int k = 0; k < curTabnoCount; k++) {
      MyJTable myjtabtmp = jtable1;
      if (k > 0) {
        myjtabtmp = (MyJTable) jtable1.getJTable(k - 1);
        myTableModel1 = (MyTableModel) myjtabtmp.getModel();
      }
      int rowcount = myTableModel1.getRowCount();

      if (rowcount == 0 || rowselects.length == 0) {
        return -1;
      } //fi(rowcount == 0 || rowselects.length ==
      else {
        for (int i = 0; i < rowselects.length; i++) {
          myTableModel1.removeRow(rowselects[rowselects.length - i - 1]);
          if (k == 0) {
            myTableModel1.managemodel.removeArec(rowselects[rowselects.length -
                                                 i - 1]);
          }
        }
      }
      myjtabtmp.updateRow();
    }
    return rowselects.length;
  }

  public int insertRow(int tabno) {
    return addRowsInsertRows(tabno, ConstantDef.DB_RECORD_INSERT);
  }

  public int addRowsInsertRows(int tabno, int flag) {

    int curTabnoCount = Integer.parseInt(vTableTabnoCount.elementAt(
        tabno).toString().trim()); //表的分区数
    int select = 0, select1 = 0;
    int random = 0, random1 = 0, random2 = 0, random3 = 0;
    char[] elect = null;
    MyJTable jtable1 = (MyJTable) vListTable.elementAt(tabno);
    if (!jtable1.getTableType().equalsIgnoreCase("U")) {
      JOptionPane.showMessageDialog(this, "不是基本表，不能编辑!");
      return -1;
    }

    MyTableModel myTableModel1 = (MyTableModel) jtable1.getModel();
    if (PreProcessor.TableLock) {
    //表互斥修改  ---洪祥，判断表是否在修改
    byte[][] buf;
    int[] fieldcount = new int[1];
    String wh;
    wh = "tablename = '" + myTableModel1.getTableName() +  "'";
    buf = pdll.queryAnyTable(ConstantDef.MODELDB, "table_status",
                             "tablename", wh, "", fieldcount, false);
    int len = buf.length;
    if (myTableModel1.getSaveFlag() == false) {
      if (len > 0) {
        JOptionPane.showMessageDialog(null, "此表正在编辑中", "提示",
                                      JOptionPane.WARNING_MESSAGE);
        return -1;
      }
    }
    //表互斥修改
    }
    if (myTableModel1.getKeyCount() < 1) {
      JOptionPane.showMessageDialog(this, "表没有关键字段，不能编辑!");
      return -1;
    }
    int[] rowselects = new int[0];
    if (curTabnoCount == 1) {
      rowselects = jtable1.getSelectedRows();
    }
    else {
      JTabbedPane tabnoJTab = (JTabbedPane) vTabnoJTab.elementAt(tabno);
      int tabno2 = tabnoJTab.getSelectedIndex();
      if (tabno2 >= 0 && tabno2 < curTabnoCount) {
        if (tabno2 == 0) {
          rowselects = jtable1.getSelectedRows();
        }
        else {
          MyJTable myjtabtmp = jtable1.getJTable(tabno2 - 1);
          rowselects = myjtabtmp.getSelectedRows();
        }
      }
    }

    for (int k = 0; k < curTabnoCount; k++) {
      MyJTable myjtabtmp = jtable1;
      if (k > 0) {
        myjtabtmp = (MyJTable) jtable1.getJTable(k - 1);
        myTableModel1 = (MyTableModel) myjtabtmp.getModel();
      }
      int col = myTableModel1.getColumnCount();
      Object[] rowData = new Object[col];
      int rowcount = myTableModel1.getRowCount();

      if (rowcount == 0 || rowselects.length == 0) {
        for (int j = 0; j < col; j++) {
          if (myTableModel1.getColumnName(j).equalsIgnoreCase("代码")) {
            select = j;
            if ( (myTableModel1.getTableName().equals("站所线系参数表")) ||
                (myTableModel1.getTableName().equals("终端参数表"))
                || (myTableModel1.getTableName().equals("通道参数表"))) {
              random = (int) (Math.random() * 32);
              random1 = (int) (Math.random() * 34);
              random2 = (int) (Math.random() * 29);
              random3 = (int) (Math.random() * 26);
              elect = new char[4];
              elect[0] = letter[random];
              elect[1] = letter[random1];
              elect[2] = letter[random2];
              elect[3] = letter[random3];
              rowData[select] = String.valueOf(elect);
            }
          }
        }
        myTableModel1.addRow(rowData);
        if (k == 0) {
          myTableModel1.managemodel.addArec();
        }
        if (myTableModel1.getListFieldFlag()) {
          myTableModel1.addListField("");
        }
      } //fi(rowcount == 0 || rowselects.length == 0)
      else {
        for (int i = 0; i < rowselects.length; i++) {
          for (int j = 0; j < col; j++) {
            if (flag == ConstantDef.DB_RECORD_ADD) { //addrows
              rowData[j] = myTableModel1.getValueAt(rowselects[i], j);
              if (myTableModel1.getColumnName(j).equalsIgnoreCase("代码")) {
                select = j;
                if ( (myTableModel1.getTableName().equals("站所线系参数表"))
                    || (myTableModel1.getTableName().equals("通道参数表"))
                    || (myTableModel1.getTableName().equals("终端参数表"))) {
                  random = (int) (Math.random() * 32);
                  random1 = (int) (Math.random() * 34);
                  random2 = (int) (Math.random() * 29);
                  random3 = (int) (Math.random() * 26);
                  elect = new char[4];
                  elect[0] = letter[random];
                  elect[1] = letter[random1];
                  elect[2] = letter[random2];
                  elect[3] = letter[random3];
                  rowData[select] = String.valueOf(elect);
                }
                else if ( (myTableModel1.getTableName().equals("SCD设备参数表"))
                         || (myTableModel1.getTableName().equals("遥信参数表"))
                         || (myTableModel1.getTableName().equals("遥测参数表"))) {
                  // rowData[select]="";
                }
                /* else if(myTableModel1.getTableName().equals("终端参数表")){
                   if (myTableModel1.getColumnName(j).equalsIgnoreCase("代码")){
                     select=j;
                   }
                 if (myTableModel1.getColumnName(j).equalsIgnoreCase("站所线系代码")){
                     select1=j;
                   }
                 rowData[select] = myTableModel1.getValueAt(rowselects[i], select1);
                 }*/
                // else{
                //  rowData[select] ="";
                // }
              }
              /*  if (myTableModel1.getColumnName(j).equalsIgnoreCase("devid")){
                  select=j;
                  String code=null;
                  byte[][] buf = new byte[0][0];
                  int[] fieldcount = new int[1];
                  if (myTableModel1.getTableName().equals("SCD设备参数表"))
                  {
                    //rowData[select]= myTableModel1.getValueAt(rowcount-1, j);
                    //rowData[select]= String.valueOf(Integer.parseInt(String.valueOf(rowData[select]))+1);
                  }
                }*/
            }
            else if (flag == ConstantDef.DB_RECORD_INSERT) { //insert rows
              rowData[j] = myTableModel1.getValueAt(rowselects[rowselects.
                  length - i - 1], j);
              if (myTableModel1.getColumnName(j).equalsIgnoreCase("代码")) {
                select = j;
                if ( (myTableModel1.getTableName().equals("站所线系参数表"))
                    || (myTableModel1.getTableName().equals("通道参数表"))
                    || (myTableModel1.getTableName().equals("终端参数表"))) {
                  random = (int) (Math.random() * 32);
                  random1 = (int) (Math.random() * 34);
                  random2 = (int) (Math.random() * 29);
                  random3 = (int) (Math.random() * 26);
                  elect = new char[4];
                  elect[0] = letter[random];
                  elect[1] = letter[random1];
                  elect[2] = letter[random2];
                  elect[3] = letter[random3];
                  rowData[select] = String.valueOf(elect);
                }
                else if ( (myTableModel1.getTableName().equals("SCD设备参数表"))
                         || (myTableModel1.getTableName().equals("遥信参数表"))
                         || (myTableModel1.getTableName().equals("遥测参数表"))) {
                  // rowData[select]="";
                }
                //   else{
                //  rowData[select]="";
                // }
              }
            }
          }
          if (flag == ConstantDef.DB_RECORD_ADD) { //addrows
            myTableModel1.addRow(rowData);
            if (k == 0) {
              myTableModel1.managemodel.addArec();
            }

            if (myTableModel1.getListFieldFlag()) {
              myTableModel1.addListField(rowselects[i]);
            }
          }
          else if (flag == ConstantDef.DB_RECORD_INSERT) { //insertrows
            myTableModel1.insertRow(rowselects[rowselects.length - i - 1] + 1,
                                    rowData);
            if (k == 0) {
              myTableModel1.managemodel.insertArec(rowselects[rowselects.length -
                  i - 1] + 1);
            }

            if (myTableModel1.getListFieldFlag()) {
              myTableModel1.insertListField(rowselects[rowselects.length - i -
                                            1]);
            }
          }
        }
      }
      myjtabtmp.updateRow();

    } //for k

    return 1;
  }

  public int saveChange(int tabno, String tablename) {  //修改记录
    int curTabnoCount = Integer.parseInt(vTableTabnoCount.elementAt(
        tabno).toString()); //表的分区数
    if (curTabnoCount <= 0) {
      return -1;
    }
    if (ConstantDef.getUserInstance().getEditPara() == 0) {
      return -1;
    }

    MyTableModel myTableModel1;
    boolean result;
    delRows(tabno, curTabnoCount);
    Vector vFailRow = new Vector(0);
    result = updateTableRows(tabno, curTabnoCount, vFailRow); //修改记录

    if (!result) {
      boolean doendf = false;
      while (!doendf) {
        Vector vFailRow1 = new Vector(0);
        boolean result1 = updateTableRows(tabno, curTabnoCount, vFailRow1);
        if (result1)
          doendf = true;
        if (vFailRow1.size() == vFailRow.size())
          doendf = true;
        result = result1;
      }
    }

    if (result) {
      MyJTable jtable1 = (MyJTable) vListTable.elementAt(tabno);
      myTableModel1 = (MyTableModel) jtable1.getModel();

      myTableModel1.setSaveFlag(false);
      if (PreProcessor.TableLock) {
      //表互斥修改   --
      String where = "";
      if (ConstantDef.Vtablename.contains(myTableModel1.getTableName())) {
        ConstantDef.Vtablename.removeElement(myTableModel1.getTableName());
        int rtn = pdll.openTableInfo(ConstantDef.MODELDB.getBytes(),
                                     "table_status".getBytes());
        if (rtn <= 0) {
          pdll.closeTableInfo();
        }
        else {
          where = "tablename = '" +myTableModel1.getTableName() + "'";
          byte[] wh = where.trim().getBytes();
          pdll.removeARecord(wh);
          pdll.closeTableInfo();
        }
      }
      //表互斥修改
      }
      if (curTabnoCount > 1) { //多个分区
        for (int i = 0; i < curTabnoCount - 1; i++) {
          MyJTable jtabtmp = jtable1.getJTable(i); //用1分区的数据
          MyTableModel mytb = (MyTableModel) jtabtmp.getModel();
          mytb.setSaveFlag(false);
        }
      }
    }
    if (!result) {
      JOptionPane.showMessageDialog(null,
                                    "保存失败，请注意调试信息！",
                                    "警告",
                                    JOptionPane.ERROR_MESSAGE);
      return -1;
    }
    return 1;
  }

  //删除某表的数据
  public int delRows(int tabno, int curTabnoCount) {

    String dbname = ConstantDef.MODELDB, tablename = "";
    int i, j, keycolcount = 0, colcount = 0, dels = 0;
    String[] keycolname;
    int[] keycoltabno; //关键列所在的区号,以关键列名的顺序存放
    int[] keycolno = new int[0]; // 关键列所在区的序号
    MyTableModel myTableModel1 = null;
    FieldProp fp;

    Vector vJtable = new Vector(0);
    Object oo;

    //得到关键字段的列名，位置
    MyJTable jtable1 = (MyJTable) vListTable.elementAt(tabno);
    myTableModel1 = (MyTableModel) jtable1.getModel();
    fp = jtable1.getTableSetInfo().tableFieldInfo;
    dels = myTableModel1.getDelRows();
    dbname = myTableModel1.getDBName();
    tablename = myTableModel1.getTableName();
    keycolcount = myTableModel1.getKeyCount();
    keycolno = new int[keycolcount];
    keycolname = myTableModel1.getkeys();
    keycoltabno = new int[keycolcount];

    for (i = 0; i < myTableModel1.getColumnCount(); i++) {
      for (j = 0; j < keycolcount; j++) {
        if (PreProcessor.ALIAS) {
          if (GetRealColName(myTableModel1.getTableName(),
                             myTableModel1.getColumnName(i)).equalsIgnoreCase(
              keycolname[j])) {
            keycoltabno[j] = 0;
            keycolno[j] = i;
            break;
          }
        }
        else {
          if (myTableModel1.getColumnName(i).equalsIgnoreCase(keycolname[j])) {
            keycoltabno[j] = 0;
            keycolno[j] = i;
            break;
          }
        }

      } //for j
    } //for i
    if (curTabnoCount > 1) { //多个分区
      for (int k = 0; k < curTabnoCount - 1; k++) {
        MyJTable jtabtmp = jtable1.getJTable(k);
        MyTableModel myTableModel2 = (MyTableModel) jtabtmp.getModel();
        for (int n = 0; n < myTableModel2.getColumnCount(); n++) { //分区列对应的总表的列数
          for (j = 0; j < keycolcount; j++) {
            if (PreProcessor.ALIAS) {
              if (GetRealColName(myTableModel2.getTableName(),
                                 myTableModel2.getColumnName(n)).
                  equalsIgnoreCase(keycolname[j])) {
                keycoltabno[j] = k + 1;
                keycolno[j] = n;
                break;
              } //if
            }
            else {
              if (myTableModel2.getColumnName(n).equalsIgnoreCase(keycolname[j])) {
                keycoltabno[j] = k + 1;
                keycolno[j] = n;
                break;
              } //if
            }
          } //for j
        } //for n
      } //for k
    } //if

    if (dels == 0) {
      return 0;
    }

    int[] fieldtype = pdll.getFieldsType(dbname, tablename, keycolname);

    byte[] tb = tablename.trim().getBytes();
    int rtn = pdll.openTableInfo(dbname.trim().getBytes(), tb);
    if (rtn <= 0) {
      pdll.closeTableInfo();
      return -1;
    }

    for (i = 0; i < dels; i++) {
      String where = "";
      Vector vtmp = new Vector(0);
      for (j = 0; j < keycolname.length; j++) {
        if (keycoltabno[j] > 0) {
          MyJTable jtabtmp = jtable1.getJTable(keycoltabno[j] - 1);
          MyTableModel mytm = (MyTableModel) jtabtmp.getModel();
          vtmp = (Vector) mytm.delData.elementAt(dels - i - 1);
        }
        else {
          vtmp = (Vector) myTableModel1.delData.elementAt(dels - i - 1);
        }

        if (vtmp.elementAt(keycolno[j]) == null) {
          continue;
        }
        if (!where.equalsIgnoreCase("")) {
          where = where + "  and  ";
        } //if

        where = where + keycolname[j];
        switch (fieldtype[j]) {
          case ConstantDef.DATATYPE:
          case ConstantDef.FLOATTYPE: { //有浮点数精度怎么办
            if (vtmp.elementAt(keycolno[j]) instanceof Boolean) { //有bug。
              boolean r = vtmp.elementAt(keycolno[j]).toString().trim().
                  equalsIgnoreCase("true");
              if (r) {
                where = where + " <> 0 ";
              }
              else {
                where = where + " = 0 ";
              }
            }
            else {
              where = where + " = " +
                  vtmp.elementAt(keycolno[j]).toString().trim();
            }
            break;
          }
          case ConstantDef.STRINGTYPE: {
            where = where + " ='" + vtmp.elementAt(keycolno[j]).toString().trim() +
                "'";
            break;
          }
        } //switch
      } //for j


      //删除记录--南网测试
      String hostname = "";
      try {
        InetAddress neta = InetAddress.getLocalHost();
        hostname = neta.getHostName();
      }
      catch (UnknownHostException nete) {
        hostname = "";
      }

      User user = ConstantDef.getUserInstance();
      String dbglogstr = "删除记录  主机名： " + hostname + "  操作员： " + user.getDesc();
      dbglogstr += "   删除条件：";
      dbglogstr += where;
      dbginfo.writelog(tablename, dbglogstr);





      byte[] wh = where.trim().getBytes();
      rtn = pdll.removeARecord(wh);
      System.out.println("删除一条记录="+String.valueOf(rtn));
      if (rtn != 0) { //删除成功
        if (tablename.equalsIgnoreCase(ConstantDef.SCDDEVTABLE)  && !PreProcessor.JINHUA ) {
          Vector v1 = (Vector) myTableModel1.delData.elementAt(dels - i - 1);
          int devid = Integer.parseInt(v1.elementAt(ConstantDef.SCDDEVIDCOLNO).
                                       toString());
          int typet = Integer.parseInt(v1.elementAt(ConstantDef.SCDDEVTYPECOLNO).
                                       toString());
          pdll.execproc(1, devid, typet, 0);
        }

        myTableModel1 = (MyTableModel) jtable1.getModel();
        myTableModel1.delData.removeElementAt(dels - i - 1);

        if (curTabnoCount > 1) {
          for (int k = 0; k < curTabnoCount - 1; k++) { //删除分区数据
            MyJTable jtabtmp = jtable1.getJTable(k); //
            MyTableModel myTableModel2 = (MyTableModel) jtabtmp.getModel();
            myTableModel2.delData.removeElementAt(dels - i - 1);
          }
        }
      }
      else {
        pdll.closeTableInfo();
        return -1;
      }
    } //for i
    pdll.closeTableInfo();
    return 1;
  }

  public boolean updateTableRows(int tabno, int curTabnoCount, Vector vFailRow) {  //修改记录
    String dbname = ConstantDef.MODELDB, tablename = "";
    int i, j, keycolcount = 0, colcount = 0, dels = 0;
    String[] keycolname;
    int[] keycoltabno; //关键列所在的区号,以关键列名的顺序存放
    int[] keycolno = new int[0]; // 关键列所在区的序号
    int rowcount = 0;
    MyTableModel myTableModel1 = null;
    FieldProp fp;
    int[] coltoallcol; //记录分区列对应的总表的列号,每区依次存放.
    //即1区+2区+...。
    Vector colname=new Vector(0); //name
    int[] coltabno;    //每列对应的区号
    int[] coltabnono;  //每列在所在区的序号
    Vector vJtable = new Vector(0);
    Object oo;
    Object oldval;     //老值--南网测试
    Object fieldname; //列名-南网测试
    vFailRow = new Vector(0); //存储失败的行

    int nocount = 0;
    //得到关键字段的列名，位置
    MyJTable jtable1 = (MyJTable) vListTable.elementAt(tabno);
    myTableModel1 = (MyTableModel) jtable1.getModel();
    fp = jtable1.getTableSetInfo().tableFieldInfo;
    dels = myTableModel1.getDelRows();
    dbname = myTableModel1.getDBName();
    tablename = myTableModel1.getTableName();
    keycolcount = myTableModel1.getKeyCount();
    keycolno = new int[keycolcount];
    colcount = jtable1.getColCount();
    coltoallcol = new int[colcount];
    coltabno = new int[colcount];
    coltabnono = new int[colcount];
    keycolname = myTableModel1.getkeys();
    keycoltabno = new int[keycolcount];
    rowcount = myTableModel1.getRowCount();

    for (i = 0; i < keycolcount; i++) { //20051108
      keycolno[i] = -1;
      keycoltabno[i] = -1;
    }

    for (i = 0; i < myTableModel1.getColumnCount(); i++) {
      for (j = 0; j < keycolcount; j++) {
        if(PreProcessor.ALIAS){
          String colName=GetRealColName(tablename,myTableModel1.getColumnName(i));
          if (colName.equalsIgnoreCase(keycolname[j])) { //获得模型的列名和关键字段比较
            keycoltabno[j] = 0; //第一区
            keycolno[j] = i;
            break;
          }
        }
        else
          if (myTableModel1.getColumnName(i).equalsIgnoreCase(keycolname[j])) {   //获得模型的列名和关键字段比较
            keycoltabno[j] = 0; //第一区
            keycolno[j] = i;
            break;
          }
      } //for j
      int tmpcol=-1;
      if(PreProcessor.ALIAS){
        tmpcol= fp.getColumnNo(GetRealColName(tablename,myTableModel1.getColumnName(i)));
        colname.addElement(GetRealColName(tablename,myTableModel1.getColumnName(i)));//南网测试
      }
      else{
        tmpcol = fp.getColumnNo(myTableModel1.getColumnName(i));
        colname.addElement(myTableModel1.getColumnName(i));//南网测试
      }
      if (tmpcol == -1) {
        JOptionPane.showMessageDialog(null,
                                      "有错误3，不能保存，请通知厂家！",
                                      "警告",
                                      JOptionPane.ERROR_MESSAGE);
        return false;
      }
      coltoallcol[i] = tmpcol;
      coltabno[i] = 0;
      coltabnono[i] = i;
      nocount++;
    } //for i
    if (curTabnoCount > 1) { //多个分区
      for (int k = 0; k < curTabnoCount - 1; k++) {
        MyJTable jtabtmp = jtable1.getJTable(k);
        MyTableModel myTableModel2 = (MyTableModel) jtabtmp.getModel();
        for (int n = 0; n < myTableModel2.getColumnCount(); n++) { //分区列对应的总表的列数
          int tmpcol=-1;
          if(PreProcessor.ALIAS){
            tmpcol = fp.getColumnNo(GetRealColName(tablename,
                myTableModel2.getColumnName(n)));
            colname.addElement(GetRealColName(tablename,
                                              myTableModel2.getColumnName(n))); //南网测试
          }
          else
          {
            tmpcol = fp.getColumnNo(myTableModel2.getColumnName(n));
            colname.addElement(
                                              myTableModel2.getColumnName(n)); //南网测试
          }
          if (tmpcol == -1) {
            JOptionPane.showMessageDialog(null,
                                          "有错误4，不能保存，请通知厂家！",
                                          "警告",
                                          JOptionPane.ERROR_MESSAGE);
            return false;
          }
          coltoallcol[nocount] = tmpcol;
          coltabno[nocount] = k + 1;
          coltabnono[nocount] = n;
          nocount++;
          for (j = 0; j < keycolcount; j++) {
            if (PreProcessor.ALIAS) {
              if (GetRealColName(myTableModel2.getTableName(),myTableModel2.getColumnName(n)).equalsIgnoreCase(keycolname[j])) {
                keycoltabno[j] = k + 1;
                keycolno[j] = n;
                break;
              } //if
            }
            else
            {
              if (myTableModel2.getColumnName(n).equalsIgnoreCase(keycolname[j])) {
                keycoltabno[j] = k + 1;
                keycolno[j] = n;
                break;
              } //if
            }
          } //for j
        } //for n
      } //for k
    } //if

    for (i = 0; i < keycolcount; i++) { //20051108
      if (keycolno[i] == -1 || keycoltabno[i] == -1) {
        JOptionPane.showMessageDialog(null,
                                      "有错误1，不能保存，请通知厂家！",
                                      "警告",
                                      JOptionPane.ERROR_MESSAGE);
        return false;
      }
    }

    boolean bisscddevtable = false;
    int maxdevid = 0;
    if (tablename.equalsIgnoreCase(ConstantDef.SCDDEVTABLE)) {
      byte flag = 0;
      if (!PreProcessor.JINHUA)
        maxdevid = pdll.getAMaxMinVal("modeldb".getBytes(),
                                      ConstantDef.SCDDEVTABLE.getBytes(),
                                      ConstantDef.SCDDEVID.getBytes(), flag);
      bisscddevtable = true;
    }

    byte[] tb = tablename.trim().getBytes();
    int rtn = pdll.openTableInfo(dbname.getBytes(), tb);
    if (rtn <= 0) {
      pdll.closeTableInfo();
      return false;
    }
    String sListFieldValue = "";

    boolean bkeytable = false;
    if (tablename.equals("遥信参数表") || tablename.equals("遥测参数表") ||
        tablename.equals("电度参数表") || tablename.equalsIgnoreCase("SCD设备参数表")
        || tablename.equals("终端参数表") || tablename.equals("通道参数表")) {
      bkeytable = true;
    }


    for (i = 0; i < rowcount; i++) {
      boolean bListFieldF = false;
      int rowp = ConstantDef.DB_UNCHANGED;
      rowp = Integer.parseInt(myTableModel1.rowProp.elementAt(i).toString().
                              trim());
      if (rowp == ConstantDef.DB_UNCHANGED) {
        for (j = 0; j < curTabnoCount - 1; j++) {
          MyJTable jtabtmp = jtable1.getJTable(j);
          MyTableModel mytm = (MyTableModel) jtabtmp.getModel();
          rowp = Integer.parseInt(mytm.rowProp.elementAt(i).toString().
                                  trim());
          if (rowp != ConstantDef.DB_UNCHANGED) {
            break;
          }
        } //for j
      }

      if (rowp == ConstantDef.DB_CHANGED) { //changed
        Vector vdata = new Vector(0); //原主键值
        Vector vnewdata = new Vector(0); //新值--南网测试
        Vector volddata=new Vector(0);//老值--南网测试
        Vector vfieldname =new Vector();//存放修改的列名--南网测试
        Vector tmpbk = new Vector(0);
        Vector tmp = new Vector(0);
        Vector vfield = new Vector(0);

        String dbgkeynames = "save关键字段名:";
        for (j = 0; j < keycolname.length; j++) {
          String columnname = "";
          int columnno = -1;

          if (keycoltabno[j] == 0) {
            tmp = (Vector) myTableModel1.dataVectorBk.elementAt(i);
            vdata.addElement(tmp.elementAt(keycolno[j])); //原主键值
            if(PreProcessor.ALIAS)
              columnname = GetRealColName(myTableModel1.getTableName(),myTableModel1.getColumnName(keycolno[j]));
            else
               columnname = myTableModel1.getColumnName(keycolno[j]);
            dbgkeynames += columnname;
            dbgkeynames += "  ";
            columnno = fp.getColumnNo(columnname);
            dbgkeynames += Integer.toString(columnno);
            dbgkeynames += "  ";
            vfield.add(Integer.toString(columnno)); //原主键值序号
          }
          else {
            MyJTable jtabtmp = jtable1.getJTable(keycoltabno[j] - 1);
            MyTableModel mytm = (MyTableModel) jtabtmp.getModel();
            tmp = (Vector) mytm.dataVectorBk.elementAt(i);
            Object o = tmp.elementAt(keycolno[j]);
            vdata.addElement(o); //原主键值
            if(PreProcessor.ALIAS)
              columnname = GetRealColName(mytm.getTableName(),mytm.getColumnName(keycolno[j]));
            else
               columnname = mytm.getColumnName(keycolno[j]);
            dbgkeynames += columnname;
            dbgkeynames += "  ";
            columnno = fp.getColumnNo(columnname);
            dbgkeynames += Integer.toString(columnno);
            dbgkeynames += "  ";
            vfield.add(Integer.toString(columnno)); //原主键值序号
          }
          if (bkeytable) {
            if (!columnname.equalsIgnoreCase("代码") || columnno < 0) {
              JOptionPane.showMessageDialog(null,
                                            "有错误2，不能保存，请通知厂家！",
                                            "警告",
                                            JOptionPane.ERROR_MESSAGE);
              return false;
            }
          }
        } //for j
      //  dbginfo.writelog(tablename, dbgkeynames);
        MyJTable jtabletmp = null;
        MyTableModel tablemodeltmp = null;
        String test1 = null, test2 = null, test3 = null, test4 = null, change = null;
        int t = 0, t1 = 0, t2 = 0;
        int random = 0, random1 = 0, random2 = 0, random3 = 0;
        for (j = 0; j < colcount; j++) {
          if (coltabno[j] == 0) { //0 区
            jtabletmp = jtable1;
            tablemodeltmp = myTableModel1;
            tmpbk = (Vector) myTableModel1.dataVectorBk.elementAt(i); //原始数据的备份
            tmp = (Vector) myTableModel1.dataVector.elementAt(i); //新数据
          }
          else {
            jtabletmp = jtable1.getJTable(coltabno[j] - 1);
            tablemodeltmp = (MyTableModel) jtabletmp.getModel();
            tmpbk = (Vector) tablemodeltmp.dataVectorBk.elementAt(i); //原始数据的备份
            tmp = (Vector) tablemodeltmp.dataVector.elementAt(i); //新数据
          }
          if (tablename.equals("SCD设备参数表")) {
            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  "类型")) {
                test1 = tmp.elementAt(coltabnono[j]).toString().trim();
                change = test1;
                test3 = tmpbk.elementAt(coltabnono[j]).toString().trim();
                t1 = j;
                System.out.print(t1);
              }
            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "类型")) {
                test1 = tmp.elementAt(coltabnono[j]).toString().trim();
                change = test1;
                test3 = tmpbk.elementAt(coltabnono[j]).toString().trim();
                t1 = j;
                System.out.print(t1);
              }
            }

            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  "devid")) {
                t2 = j;
                System.out.print(t2);
              }
            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "devid")) {
                t2 = j;
                System.out.print(t2);
              }
            }

            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),
                                 tablemodeltmp.getColumnName(coltabnono[j])).
                  equalsIgnoreCase(
                      "站所线系代码")) {
                test2 = tmp.elementAt(coltabnono[j]).toString().trim();
                test4 = tmpbk.elementAt(coltabnono[j]).toString().trim();
              }
            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "站所线系代码")) {
                test2 = tmp.elementAt(coltabnono[j]).toString().trim();
                test4 = tmpbk.elementAt(coltabnono[j]).toString().trim();
              }
            }

            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),
                                 tablemodeltmp.getColumnName(coltabnono[j])).
                  equalsIgnoreCase(
                      "代码")) {
                t = j;
              }

            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "代码")) {
                t = j;
              }

            }

            random = (int) (Math.random() * 26);
            random1 = (int) (Math.random() * 29);
            random2 = (int) (Math.random() * 33);
            char[] test = new char[3];
            test[0] = letter[random];
            test[1] = letter[random1];
            test[2] = letter[random2];
            if (test1 != null && test2 != null) {

              if (test1.equals("0")) {
                test1 = "";
              }
              else if (test1.equals("1")) {
                test1 = "K";
              }
              else if (test1.equals("2")) {
                test1 = "D";
              }
              else if (test1.equals("3")) {
                test1 = "";
              }
              else if (test1.equals("4")) {
                test1 = "K";
              }
              else if (test1.equals("5")) {
                test1 = "B";
              }
              else if (test1.equals("6")) {
                test1 = "b";
              }
              else if (test1.equals("7")) {
                test1 = "C";
              }
              else if (test1.equals("8")) {
                test1 = "R";
              }
              else if (test1.equals("9")) {
                test1 = "Q";
              }
              else if (test1.equals("10")) {
                test1 = "U";
              }
              else if (test1.equals("11")) {
                test1 = "I";
              }
              else if (test1.equals("12")) {
                test1 = "L";
              }
              else if (test1.equals("13")) {
                test1 = "D";
              }
              else if (test1.equals("14")) {
                test1 = "X";
              }
              else if (test1.equals("15")) {
                test1 = "x";
              }
              else if (test1.equals("16")) {
                test1 = "M";
              }
              else if (test1.equals("17")) {
                test1 = "F";
              }
              else if (test1.equals("18")) {
                test1 = "J";
              }
              else if (test1.equals("19")) {
                test1 = "J";
              }
              else if (test1.equals("20")) {
                test1 = "J";
              }
              else if (test1.equals("21")) {
                test1 = "H";
              }
              else if (test1.equals("22")) {
                test1 = "P";
              }
              else if (test1.equals("23")) {
                test1 = "K";
              }
              else if (test1.equals("25")) {
                  test1 = "O";
              }
              else if (test1.equals("26")) {
                  test1 = "K";
              }
              else if (test1.equals("27")) {
                  test1 = "K";
              }
              else if (test1.equals("28")) {
                  test1 = "K";
              }
              else if (test1.equals("29")) {
                  test1 = "O";
              }
              else if (test1.equals("30")) {
                  test1 = "O";
              }
              else if (test1.equals("51")) {
                test1 = "O";
              }
              else if (test1.equals("52")) {
                test1 = "S";
              }
              if(PreProcessor.ALIAS){
                if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                    "代码")) {
                  if (!change.equalsIgnoreCase(test3) ||
                      !test2.equalsIgnoreCase(test4))
                    tablemodeltmp.setValueAt(test1 + test2 +
                                             String.valueOf(test),
                                             i, keycolno[0]);
                }
              }
              else{
                if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                    "代码")) {
                  if (!change.equalsIgnoreCase(test3) ||
                      !test2.equalsIgnoreCase(test4))
                    tablemodeltmp.setValueAt(test1 + test2 +
                                             String.valueOf(test),
                                             i, keycolno[0]);
                }
              }
                //修改SCD设备参数表自动修改代码
//                tablemodeltmp.setValueAt(test1 + test2 + String.valueOf(test), i, keycolno[0]);
            }
          }
          if(PreProcessor.MD5){
            if (tablename.equals("用户口令参数表")) {
              if (tablemodeltmp.getColumnName(coltabnono[j]).
                  equalsIgnoreCase(
                      "密码")) {
                test1 = tmp.elementAt(coltabnono[j]).toString().trim(); //新密码
                test2 = tmpbk.elementAt(coltabnono[j]).toString().trim(); //老密码
                if (!test1.equalsIgnoreCase(test2)) {
                  Md5Str md5str = new Md5Str();
                  tablemodeltmp.setValueAt(md5str.PWDtoMD5(test1),
                                           i, j);
                }
                // colvalue[j] = md5str.PWDtoMD5(test1).getBytes();
              }
            }
            if (tablename.equals("web用户表")) {
              if (tablemodeltmp.getColumnName(coltabnono[j]).
                  equalsIgnoreCase(
                      "口令")) {
                test1 = tmp.elementAt(coltabnono[j]).toString().trim();
                test2 = tmpbk.elementAt(coltabnono[j]).toString().trim(); //老密码
                if (!test1.equalsIgnoreCase(test2)) {
                  Md5Str md5str = new Md5Str();
                  tablemodeltmp.setValueAt(md5str.PWDtoMD5(test1),
                                           i, j);
                }
                // colvalue[j] = md5str.PWDtoMD5(test1).getBytes();
              }
            }
          }
          if (tablename.equals("遥信参数表") || tablename.equals("遥测参数表") ||
              tablename.equals("电度参数表")) {
            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  "设备代码")) {
                test1 = tmp.elementAt(coltabnono[j]).toString().trim();
              }

            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "设备代码")) {
                test1 = tmp.elementAt(coltabnono[j]).toString().trim();
              }
            }

            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  "设备代码")) {
                test1 = tmp.elementAt(coltabnono[j]).toString().trim();
              }
            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "设备代码")) {
                test1 = tmp.elementAt(coltabnono[j]).toString().trim();
              }
            }

            //   if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
            //    "终端序号")) {
            //     test2 =  tmp.elementAt(coltabnono[j]).toString().trim();
            //  }

            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  "遥信序号") ||
                  GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                      "遥测序号") ||
                  GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                      "电度序号")) {
                test3 = tmp.elementAt(coltabnono[j]).toString().trim();
              }
            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "遥信序号") ||
                  tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                      "遥测序号") ||
                  tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                      "电度序号")) {
                test3 = tmp.elementAt(coltabnono[j]).toString().trim();
              }
             }

             if (PreProcessor.ALIAS) {
               if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                   "类型")) {
                 test4 = tmp.elementAt(coltabnono[j]).toString().trim();
               }
             }
             else {
               if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                   "类型")) {
                 test4 = tmp.elementAt(coltabnono[j]).toString().trim();
               }
              }

              if (PreProcessor.ALIAS) {

              }
              else{

              }

              boolean bIsCode = false;
              if (PreProcessor.ALIAS) {
                if (GetRealColName(tablemodeltmp.getTableName(),
                                   tablemodeltmp.getColumnName(coltabnono[j])).
                    equalsIgnoreCase("代码"))
                  bIsCode = true;
              }
              else {
                if (tablemodeltmp.getColumnName(coltabnono[j]).
                    equalsIgnoreCase(
                        "代码"))
                  bIsCode = true;
              }
            if (bIsCode) {
              t = j;
              if ( (test1 != null) && (test3 != null) &&
                  (test4 != null)) {
                if (test4.equals("0")) {
                  test4 = "Dz1";
                }
                else if (test4.equals("1")) {
                  test4 = "I";
                }
                else if (test4.equals("2")) {
                  test4 = "Ia";
                }
                else if (test4.equals("3")) {
                  test4 = "Ib";
                }
                else if (test4.equals("4")) {
                  test4 = "Ic";
                }
                else if (test4.equals("5")) {
                  test4 = "Id";
                }
                else if (test4.equals("6")) {
                  test4 = "Ie";
                }
                else if (test4.equals("7")) {
                  test4 = "If";
                }
                else if (test4.equals("8")) {
                  test4 = "I0";
                }
                else if (test4.equals("9")) {
                  test4 = "I1";
                }
                else if (test4.equals("10")) {
                  test4 = "I2";
                }
                else if (test4.equals("11")) {
                  test4 = "I3";
                }
                else if (test4.equals("20")) {
                  test4 = "P";
                }
                else if (test4.equals("21")) {
                  test4 = "Pa";
                }
                else if (test4.equals("22")) {
                  test4 = "Pb";
                }
                else if (test4.equals("23")) {
                  test4 = "Pc";
                }
                else if (test4.equals("24")) {
                  test4 = "Pd";
                }
                else if (test4.equals("25")) {
                  test4 = "Pe";
                }
                else if (test4.equals("26")) {
                  test4 = "Pf";
                }
                else if (test4.equals("27")) {
                  test4 = "P1";
                }
                else if (test4.equals("28")) {
                  test4 = "P2";
                }
                else if (test4.equals("29")) {
                  test4 = "P3";
                }
                else if (test4.equals("40")) {
                  test4 = "Q";
                }
                else if (test4.equals("41")) {
                  test4 = "Qa";
                }
                else if (test4.equals("42")) {
                  test4 = "Qb";
                }
                else if (test4.equals("43")) {
                  test4 = "Qc";
                }
                else if (test4.equals("44")) {
                  test4 = "Qd";
                }
                else if (test4.equals("45")) {
                  test4 = "Qe";
                }
                else if (test4.equals("46")) {
                  test4 = "Qf";
                }
                else if (test4.equals("47")) {
                  test4 = "Q1";
                }
                else if (test4.equals("48")) {
                  test4 = "Q2";
                }
                else if (test4.equals("49")) {
                  test4 = "Q3";
                }
                else if (test4.equals("60")) {
                  test4 = "S";
                }
                else if (test4.equals("61")) {
                  test4 = "Cs";
                }
                //----新加类型
                else if(test4.equals("62")){
                  test4="I1";
                }
                else if (test4.equals("63")) {
                  test4 = "I2";
                }
                else if (test4.equals("64")) {
                  test4 = "I3";
                }
                else if (test4.equals("65")) {
                  test4 = "Ia1";
                }
                else if (test4.equals("66")) {
                  test4 = "Ia2";
                }
                else if (test4.equals("67")) {
                  test4 = "Ia3";
                }
                else if (test4.equals("68")) {
                  test4 = "Ib1";
                }
                else if (test4.equals("69")) {
                  test4 = "Ib2";
                }
                else if (test4.equals("70")) {
                  test4 = "Ib3";
                }
                else if (test4.equals("71")) {
                  test4 = "Ic1";
                }
                else if (test4.equals("72")) {
                  test4 = "Ic2";
                }
                else if (test4.equals("73")) {
                  test4 = "Ic3";
                }
                else if (test4.equals("74")) {
                  test4 = "P1";
                }
                else if (test4.equals("75")) {
                  test4 = "P2";
                }
                else if (test4.equals("76")) {
                  test4 = "P3";
                }
                else if (test4.equals("77")) {
                  test4 = "Pa1";
                }
                else if (test4.equals("78")) {
                  test4 = "Pa2";
                }
                else if (test4.equals("79")) {
                  test4 = "Pa3";
                }
                else if (test4.equals("80")) {
                  test4 = "Pb1";
                }
                else if (test4.equals("81")) {
                  test4 = "Pb2";
                }
                else if (test4.equals("82")) {
                  test4 = "Pb3";
                }
                else if (test4.equals("83")) {
                  test4 = "Pc1";
                }
                else if (test4.equals("84")) {
                  test4 = "Pc2";
                }
                else if (test4.equals("85")) {
                  test4 = "Pc3";
                }
                else if (test4.equals("86")) {
                  test4 = "Q1";
                }
                else if (test4.equals("87")) {
                  test4 = "Q2";
                }
                else if (test4.equals("88")) {
                  test4 = "Q3";
                }
                else if (test4.equals("89")) {
                  test4 = "Qa1";
                }
                else if (test4.equals("90")) {
                  test4 = "Qa2";
                }
                else if (test4.equals("91")) {
                  test4 = "Qa3";
                }
                else if (test4.equals("92")) {
                  test4 = "Qb1";
                }
                else if (test4.equals("93")) {
                  test4 = "Qb2";
                }
                else if (test4.equals("94")) {
                  test4 = "Qb3";
                }
                else if (test4.equals("95")) {
                  test4 = "Qc1";
                }
                else if (test4.equals("96")) {
                  test4 = "Qc2";
                }
                else if (test4.equals("97")) {
                  test4 = "Qc3";
                }
                else if (test4.equals("101")) {
                  test4 = "Pl";
                }
                else if (test4.equals("102")) {
                  test4 = "Wd";
                }
                else if (test4.equals("103")) {
                  test4 = "Yl";
                }
                else if (test4.equals("104")) {
                  test4 = "Sw";
                }
                else if (test4.equals("105")) {
                  test4 = "Ll";
                }
                else if (test4.equals("106")) {
                  test4 = "Sd";
                }
                else if (test4.equals("107")) {
                  test4 = "Dw";
                }
                else if (test4.equals("108")) {
                  test4 = "T0";
                }
                else if (test4.equals("109")) {
                  test4 = "T1";
                }
                else if (test4.equals("110")) {
                  test4 = "T2";
                }
                else if (test4.equals("127")) {
                  test4 = "Yc";
                }
                else if (test4.equals("150")) {
                  test4 = "V";
                }
                else if (test4.equals("151")) {
                  test4 = "Va";
                }
                else if (test4.equals("152")) {
                  test4 = "Vb";
                }
                else if (test4.equals("153")) {
                  test4 = "Vc";
                }
                else if (test4.equals("154")) {
                  test4 = "Vd";
                }
                else if (test4.equals("155")) {
                  test4 = "Ve";
                }
                else if (test4.equals("156")) {
                  test4 = "Vf";
                }
                else if (test4.equals("157")) {
                  test4 = "V0";
                }
                else if (test4.equals("158")) {
                  test4 = "V1";
                }
                else if (test4.equals("159")) {
                  test4 = "V2";
                }
                else if (test4.equals("200")) {
                  test4 = "Wh";
                }
                else if (test4.equals("201")) {
                  test4 = "Vh";
                }
                else if (test4.equals("202")) {
                  test4 = "Wp";
                }
                else if (test4.equals("203")) {
                  test4 = "Wn";
                }
                else if (test4.equals("204")) {
                  test4 = "Vp";
                }
                else if (test4.equals("205")) {
                  test4 = "Vn";
                }
                else if (test4.equals("206")) {
                  test4 = "Rt";
                }
                else if ( (test4.equals("300")) || (test4.equals("301")) ||
                         (test4.equals("302")) || (test4.equals("303")) ||
                         (test4.equals("304") || (test4.equals("305")))) {
                  test4 = "Kg";
                }
                else if (test4.equals("400")) {
                  test4 = "Zt";
                }
                else if ( (test4.equals("500")) || (test4.equals("501")) ||
                         (test4.equals("502")) || (test4.equals("503")) ||
                         (test4.equals("504")) || (test4.equals("505")) ||
                         (test4.equals("506")) || (test4.equals("507")) ||
                         (test4.equals("508")) || (test4.equals("509")) ||
                         (test4.equals("510")) || (test4.equals("541")) ||
                         (test4.equals("542")) || (test4.equals("543")) ||
                         (test4.equals("544")) || (test4.equals("571")) ||
                         (test4.equals("572") || (test4.equals("500")))) {
                  test4 = "Dz";
                }
                else if (test4.equals("600")) {
                  test4 = "Bh";
                }
                else if (test4.equals("601")) {
                  test4 = "Sg";
                }
                else if (test4.equals("602")) {
                  test4 = "Yg";
                }
                else if (test4.equals("603")) {
                    test4 = "Bh";
                }
                else if (test4.equals("604")) {
                    test4 = "Bh";
                }
                else if (test4.equals("605")) {
                    test4 = "Bh";
                }
                else if (test4.equals("606")) {
                    test4 = "Bh";
                }
                else if (test4.equals("607")) {
                    test4 = "Bh";
                }
                else if (test4.equals("608")) {
                    test4 = "Bh";
                }
                else if ( (test4.equals("4001")) || (test4.equals("4002")) ||
                         (test4.equals("4003")) || (test4.equals("4004")) ||
                         (test4.equals("4005")) || (test4.equals("4011")) ||
                         (test4.equals("4012")) || (test4.equals("4013")) ||
                         (test4.equals("4014")) || (test4.equals("4015")) ||
                         (test4.equals("4016")) || (test4.equals("4017")) ||
                         (test4.equals("4018")) || (test4.equals("4019")) ||
                         (test4.equals("4020")) || (test4.equals("4021")) ||
                         (test4.equals("4020")) || (test4.equals("4021")) ||
                         (test4.equals("4033")) || (test4.equals("4034")) ||
                         (test4.equals("4035")) || (test4.equals("4036")) ||
                         (test4.equals("4031") || (test4.equals("4032")) ||
                          (test4.equals("4037")))) {
                  test4 = "Da";
                }
                else if ( (test4.equals("4101")) || (test4.equals("4102")) ||
                         (test4.equals("4103")) || (test4.equals("4104")) ||
                         (test4.equals("4105")) || (test4.equals("4111")) ||
                         (test4.equals("4112")) || (test4.equals("4113")) ||
                         (test4.equals("4114")) || (test4.equals("4115")) ||
                         (test4.equals("4121")) || (test4.equals("4122")) ||
                         (test4.equals("4123")) || (test4.equals("4124")) ||
                         (test4.equals("4125")) || (test4.equals("4126")) ||
                         (test4.equals("4127")) || (test4.equals("4128")) ||
                         (test4.equals("4129")) || (test4.equals("4130")) ||
                         (test4.equals("4131")) || (test4.equals("4132")) ||
                         (test4.equals("4141")) || (test4.equals("4142")) ||
                         (test4.equals("4141")) || (test4.equals("4142")) ||
                         (test4.equals("4143")) || (test4.equals("4144")) ||
                         (test4.equals("4145")) || (test4.equals("4146")) ||
                         (test4.equals("4147")) || (test4.equals("4148")) ||
                         (test4.equals("4149")) || (test4.equals("4150")) ||
                         (test4.equals("4151"))) {
                  test4 = "Ftu";
                }
                //20080901新疆系统需要602、400类型加遥信号修改如下代码
                if (test4.equalsIgnoreCase("Bh")) {
                  tablemodeltmp.setValueAt(test1 + test3 + test4, i, keycolno[0]);
                }
                else if (test4.equalsIgnoreCase("Yj")
                        || test4.equalsIgnoreCase("Cd")
                        || test4.equalsIgnoreCase("Cn")
                        || test4.equalsIgnoreCase("Yc")
                        ) {
                      tablemodeltmp.setValueAt(test1 + test3 + test4, i, keycolno[0]);
                }
                else if (PreProcessor.XINJIANG &&
                         (test4.equalsIgnoreCase("Yg") ||
                          test4.equalsIgnoreCase("Zt"))) {
                  tablemodeltmp.setValueAt(test1 + test3 + test4, i, keycolno[0]);
                }
                else {
                  tablemodeltmp.setValueAt(test1 + test4, i, keycolno[0]);
                }
                /*if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                    "代码")) {
                  if (test1.substring(0,1).equalsIgnoreCase("P"))
                  {
                 tablemodeltmp.setValueAt(test1 + test3 + test4,i,keycolno[0]);
                  }else{
                  tablemodeltmp.setValueAt(test1 + test4,i,keycolno[0]);
                  }
                                     }*/
              }
            }
          }


          oo = tmp.elementAt(coltabnono[j]);
          oldval=tmpbk.elementAt(coltabnono[j]);    //老值--南网测试


          if (tmpbk.elementAt(coltabnono[j]) == null && oo == null) {
            continue;
          }
          if ( (tmpbk.elementAt(coltabnono[j]) == null && oo != null) ||
              tmpbk.elementAt(coltabnono[j]).toString().trim().equals(oo.
              toString().trim()) != true) {
            vdata.addElement(oo); //新值
            vnewdata.addElement(oo);//老值--南网测试
            volddata.addElement(oldval);//老值--南网测试
            vfieldname.addElement(colname.elementAt(j));
            vfield.addElement(Integer.toString(coltoallcol[j])); //新值的列号
            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  tablemodeltmp.getListField())) {
                sListFieldValue = oo.toString().trim();
                bListFieldF = true;
              }
            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  tablemodeltmp.getListField())) {
                sListFieldValue = oo.toString().trim();
                bListFieldF = true;
              }
            }
          }
        }
        //修改记录保存--南网测试
        int veditsize=vnewdata.size();

        if(veditsize > 0)
        {

          String hostname = "";
          try {
            InetAddress neta = InetAddress.getLocalHost();
            hostname = neta.getHostName();
          }
          catch (UnknownHostException nete) {
            hostname = "";
          }
          User user = ConstantDef.getUserInstance();
          String dbglogstr = "  主机名： " + hostname + "  操作员： " + user.getDesc();
          for(j = 0;j < veditsize;j++)
          {
            dbglogstr += "  列名： " + vfieldname.elementAt(j).toString() +"  "+ volddata.elementAt(j).toString() + "  ->>  " +vnewdata.elementAt(j).toString();
            dbglogstr += "  ";
          }
          dbginfo.writelog(tablename, dbglogstr);
        }
        //--修改记录保存

        int vdatasize = vdata.size();
        if (vdatasize > keycolcount) { //说明有列值的修改
          byte[][] colvalue = new byte[vdatasize][0];
          int[] field = new int[vdatasize];
          for (j = 0; j < vdatasize; j++) {
            field[j] = -1; //20051108
            if (vdata.elementAt(j) == null) {
              continue;
            }
            if (vdata.elementAt(j) instanceof Boolean) {
              boolean r = vdata.elementAt(j).toString().trim().equalsIgnoreCase(
                  "true");
              if (r) {
                colvalue[j] = "1".getBytes();
              }
              else {
                colvalue[j] = "0".getBytes();
              }
            }
            else {
              colvalue[j] = vdata.elementAt(j).toString().trim().getBytes();
            }
            field[j] = Integer.parseInt(vfield.elementAt(j).toString().trim());
          } //for j
          String dbglogstr = "updateARecord关键字段列号：";
          for (int dbg = 0; dbg < keycolcount; dbg++) {
            dbglogstr += Integer.toString(field[dbg]);
            dbglogstr += "  ";
          }
          //dbginfo.writelog(tablename, dbglogstr);

          if(PreProcessor.ALIAS){ //因为别名修改的缘故，添加这部分防止发生更新表出错的情况。
            tb = tablename.trim().getBytes();
            rtn = pdll.openTableInfo(dbname.getBytes(), tb);
            if (rtn <= 0) {
              pdll.closeTableInfo();
              return false;
            }
          }
          int pdllrtn = pdll.updateARecord(colvalue, field, keycolcount);
        //  System.out.println("更新一条记录="+String.valueOf(pdllrtn));
          if (pdllrtn != 0) { //修改成功后，要改rowProp
            if (bisscddevtable && !PreProcessor.JINHUA) { //ConstantDef.SCDDEVIDCOLNO
              //ConstantDef.SCDDEVTYPECOLNO
              int devid = Integer.parseInt(tablemodeltmp.getValueAt(i,
                  ConstantDef.SCDDEVIDCOLNO).toString());
              int devt = Integer.parseInt(tablemodeltmp.getValueAt(i,
                  ConstantDef.SCDDEVTYPECOLNO).toString());
              int olddevt = 0;
              if (tmpbk != null) {
                olddevt = Integer.parseInt(tmpbk.elementAt(ConstantDef.
                    SCDDEVTYPECOLNO).toString());
              }
              pdll.execproc(2, devid, devt, olddevt);
            }
            myTableModel1.rowProp.setElementAt(Integer.toString(ConstantDef.
                DB_UNCHANGED).trim(), i);
            myTableModel1.dataVectorBk.setElementAt( ( (Vector) (
                myTableModel1.dataVector.elementAt(i))).clone(), i);
            myTableModel1.managemodel.setUnDoFlag(i);
            if (bListFieldF) {
              myTableModel1.setListField(i, sListFieldValue);
            }
            jtable1.updateRow();
            if (curTabnoCount > 1) { //多个表分区
              for (j = 0; j < curTabnoCount - 1; j++) {
                MyJTable jtabtmp = jtable1.getJTable(j); //用1分区的数据
                MyTableModel myjt = (MyTableModel) jtabtmp.getModel();
                myjt.rowProp.setElementAt(Integer.toString(ConstantDef.
                    DB_UNCHANGED).trim(), i);
                myjt.dataVectorBk.setElementAt( ( (Vector) (
                    myjt.dataVector.elementAt(i))).clone(), i);
                if (bListFieldF) {
                  myjt.setListField(i, sListFieldValue);
                }
                jtabtmp.updateRow();
              }
            } //if(curTabnoCount >1)
          } //if(pdll.updateARecord(colvalue,field,keys) >0)//修改成功后，要改rowProp
          else {
            //  pdll.closeTableInfo();
            //  return -1 * i;
            vFailRow.add(Integer.toString(i));
            continue;
          } //else
        } //if(vdatasize >keys)//说明有列值的修改
      } //if(Integer.parseInt(rowProp.elementAt(i) .toString() )==2)//changed
      else if (rowp == ConstantDef.DB_INSERT) { //insert

        int devtype = 0; //设备类型
        boolean bdevidhas = false;
        int fieldnums = fp.fieldnums;
        byte[][] colvalue = new byte[fieldnums][0];
        Vector vdata = new Vector(0);
        Vector tmp = new Vector(0);
        String test1 = null, test2 = null, test3 = null, test4 = null, all = null,
            statment = null, code = null;
        MyJTable jtabletmp = null;
        MyTableModel tablemodeltmp = null;
        int random = 0, random1 = 0, random2 = 0, random3 = 0;
        int t = 0, len = 0, t0 = 0;
        byte[][] buf = new byte[0][0];
        int[] fieldcount = new int[1];
        for (j = 0; j < colcount; j++) {
          if (coltabno[j] == 0) { //0 区
            jtabletmp = jtable1;
            tablemodeltmp = myTableModel1;
            tmp = (Vector) myTableModel1.dataVector.elementAt(i); //新数据
          }
          else {
            jtabletmp = jtable1.getJTable(coltabno[j] - 1);
            tablemodeltmp = (MyTableModel) jtabletmp.getModel();
            tmp = (Vector) tablemodeltmp.dataVector.elementAt(i); //新数据
          }
          String bb = "";
          if (PreProcessor.ALIAS)
            bb = GetRealColName(myTableModel1.getTableName(),
                                myTableModel1.getColumnName(j));
          else
            bb = myTableModel1.getColumnName(j);
          String ss = fp.getTableFieldName(j);

          String mm="";
          if (PreProcessor.ALIAS)
            mm=GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j]));
          else
            mm=tablemodeltmp.getColumnName(coltabnono[j]);
          oo = tmp.elementAt(coltabnono[j]);


          if (tablename.equals("SCD设备参数表")) {
            // if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
            //   "代码")) {
            //  t=j;
            // }
            if (oo == null) {
              if (PreProcessor.ALIAS) {
                if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                    "类型")) {
                  if (oo == null) {
                    JOptionPane.showMessageDialog(null,
                                                  "您没有填写类型！",
                                                  "警告",
                                                  JOptionPane.ERROR_MESSAGE);
                    return true;
                  }
                  else {
                    test1 = tmp.elementAt(coltabnono[j]).toString().trim();
                  }
                }
              }
              else {
                if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                    "类型")) {
                  if (oo == null) {
                    JOptionPane.showMessageDialog(null,
                                                  "您没有填写类型！",
                                                  "警告",
                                                  JOptionPane.ERROR_MESSAGE);
                    return true;
                  }
                  else {
                    test1 = tmp.elementAt(coltabnono[j]).toString().trim();
                  }
                }
              }

              if (PreProcessor.ALIAS) {
                if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                    "站所线系代码")) {
                  if (oo == null) {
                    JOptionPane.showMessageDialog(null,
                                                  "您没有填写站所线系代码！",
                                                  "警告",
                                                  JOptionPane.ERROR_MESSAGE);
                    return true;
                  }
                  else {
                    test2 = tmp.elementAt(coltabnono[j]).toString().trim();
                  }
                }
              }
              else {
                if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                    "站所线系代码")) {
                  if (oo == null) {
                    JOptionPane.showMessageDialog(null,
                                                  "您没有填写站所线系代码！",
                                                  "警告",
                                                  JOptionPane.ERROR_MESSAGE);
                    return true;
                  }
                  else {
                    test2 = tmp.elementAt(coltabnono[j]).toString().trim();
                  }
                }
              }

              random = (int) (Math.random() * 32);
              random1 = (int) (Math.random() * 34);
              random2 = (int) (Math.random() * 29);
              char[] test = new char[3];
              test[0] = letter[random];
              test[1] = letter[random1];
              test[2] = letter[random2];
              if (test1 != null && test2 != null) {
                // if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                //  "代码")) {
                if (test1.equals("0")) {
                  test1 = "";
                }
                else if (test1.equals("1")) {
                  test1 = "K";
                }
                else if (test1.equals("2")) {
                  test1 = "D";
                }
                else if (test1.equals("3")) {
                  test1 = "";
                }
                else if (test1.equals("4")) {
                  test1 = "K";
                }
                else if (test1.equals("5")) {
                  test1 = "B";
                }
                else if (test1.equals("6")) {
                  test1 = "b";
                }
                else if (test1.equals("7")) {
                  test1 = "C";
                }
                else if (test1.equals("8")) {
                  test1 = "R";
                }
                else if (test1.equals("9")) {
                  test1 = "Q";
                }
                else if (test1.equals("10")) {
                  test1 = "U";
                }
                else if (test1.equals("11")) {
                  test1 = "I";
                }
                else if (test1.equals("12")) {
                  test1 = "L";
                }
                else if (test1.equals("13")) {
                  test1 = "D";
                }
                else if (test1.equals("14")) {
                  test1 = "X";
                }
                else if (test1.equals("15")) {
                  test1 = "x";
                }
                else if (test1.equals("16")) {
                  test1 = "M";
                }
                else if (test1.equals("17")) {
                  test1 = "F";
                }
                else if (test1.equals("18")) {
                  test1 = "J";
                }
                else if (test1.equals("19")) {
                  test1 = "J";
                }
                else if (test1.equals("20")) {
                  test1 = "J";
                }
                else if (test1.equals("21")) {
                  test1 = "H";
                }
                else if (test1.equals("22")) {
                  test1 = "P";
                }
                else if (test1.equals("23")) {
                  test1 = "K";
                }
                else if (test1.equals("25")) {
                    test1 = "O";
                }
                else if (test1.equals("26")) {
                    test1 = "K";
                }
                else if (test1.equals("27")) {
                    test1 = "K";
                }
                else if (test1.equals("28")) {
                    test1 = "K";
                }
                else if (test1.equals("29")) {
                    test1 = "O";
                }
                else if (test1.equals("30")) {
                    test1 = "O";
                }
                else if (test1.equals("51")) {
                  test1 = "O";
                }
                else if (test1.equals("52")) {
                  test1 = "S";
                }
                if (PreProcessor.ALIAS) {
                  if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).
                      equalsIgnoreCase(
                          "代码")) {
                    t = j;
                    if (oo == null) {
                      colvalue[coltoallcol[t]] = (test1 + test2 +
                                                  String.valueOf(test)).
                          getBytes();
                      if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).
                          equalsIgnoreCase(
                              "代码")) {
                        tablemodeltmp.setValueAt(test1 + test2 +
                                                 String.valueOf(test), i,
                                                 keycolno[0]);
                      }
                    }
                  }
                }
                else {
                  if (tablemodeltmp.getColumnName(coltabnono[j]).
                      equalsIgnoreCase(
                          "代码")) {
                    t = j;
                    if (oo == null) {
                      colvalue[coltoallcol[t]] = (test1 + test2 +
                                                  String.valueOf(test)).
                          getBytes();
                      if (tablemodeltmp.getColumnName(coltabnono[j]).
                          equalsIgnoreCase(
                              "代码")) {
                        tablemodeltmp.setValueAt(test1 + test2 +
                                                 String.valueOf(test), i,
                                                 keycolno[0]);
                      }
                    }
                  }
                }
                  //新增SCD设备参数时生成代码
                colvalue[coltoallcol[t]] = (test1+test2+String.valueOf(test)).getBytes();
                tablemodeltmp.setValueAt(test1+test2+String.valueOf(test),i,keycolno[0]);
              }
            }
          }
          if (tablename.equals("遥信参数表") || tablename.equals("遥测参数表") ||
              tablename.equals("电度参数表")) {
            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  "设备代码")) {
                if (oo == null) {
                  JOptionPane.showMessageDialog(null,
                                                "您没有填写设备代码！",
                                                "警告",
                                                JOptionPane.ERROR_MESSAGE);
                  return true;
                }
                else {
                  test1 = oo.toString().trim();
                }
              }
            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "设备代码")) {
                if (oo == null) {
                  JOptionPane.showMessageDialog(null,
                                                "您没有填写设备代码！",
                                                "警告",
                                                JOptionPane.ERROR_MESSAGE);
                  return true;
                }
                else {
                  test1 = oo.toString().trim();
                }
              }
            }

            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  "终端序号")) {
                if (oo == null) {
                  JOptionPane.showMessageDialog(null,
                                                "您没有填写终端序号！",
                                                "警告",
                                                JOptionPane.ERROR_MESSAGE);
                  return true;
                }
                else {
                  test2 = oo.toString().trim();
                }
              }
            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "终端序号")) {
                if (oo == null) {
                  JOptionPane.showMessageDialog(null,
                                                "您没有填写终端序号！",
                                                "警告",
                                                JOptionPane.ERROR_MESSAGE);
                  return true;
                }
                else {
                  test2 = oo.toString().trim();
                }
              }
            }

            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  "遥信序号") ||
                  GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                      "遥测序号") ||
                  GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                      "电度序号")) {
                if (oo == null) {
                  test3 = Integer.toString(0).trim();
                }
                else {
                  test3 = oo.toString().trim();
                }
              }

            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "遥信序号") ||
                  tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                      "遥测序号") ||
                  tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                      "电度序号")) {
                if (oo == null) {
                  test3 = Integer.toString(0).trim();
                }
                else {
                  test3 = oo.toString().trim();
                }
              }

            }

            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  "类型")) {
                if (oo == null) {
                  JOptionPane.showMessageDialog(null,
                                                "您没有填写类型！",
                                                "警告",
                                                JOptionPane.ERROR_MESSAGE);
                  return true;
                }
                else {
                  test4 = oo.toString().trim();
                }
              }
            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  "类型")) {
                if (oo == null) {
                  JOptionPane.showMessageDialog(null,
                                                "您没有填写类型！",
                                                "警告",
                                                JOptionPane.ERROR_MESSAGE);
                  return true;
                }
                else {
                  test4 = oo.toString().trim();
                }
              }
            }


            if ( (test1 != null) && (test3 != null) &&
                (test4 != null)) {
              if (test4.equals("0")) {
                test4 = "Dz1";
              }
              else if (test4.equals("1")) {
                test4 = "I";
              }
              else if (test4.equals("2")) {
                test4 = "Ia";
              }
              else if (test4.equals("3")) {
                test4 = "Ib";
              }
              else if (test4.equals("4")) {
                test4 = "Ic";
              }
              else if (test4.equals("5")) {
                test4 = "Id";
              }
              else if (test4.equals("6")) {
                test4 = "Ie";
              }
              else if (test4.equals("7")) {
                test4 = "If";
              }
              else if (test4.equals("8")) {
                test4 = "I0";
              }
              else if (test4.equals("9")) {
                test4 = "I1";
              }
              else if (test4.equals("10")) {
                test4 = "I2";
              }
              else if(test4.equals("11")){
                test4="I3";
              }
              else if (test4.equals("20")) {
                test4 = "P";
              }
              else if (test4.equals("21")) {
                test4 = "Pa";
              }
              else if (test4.equals("22")) {
                test4 = "Pb";
              }
              else if (test4.equals("23")) {
                test4 = "Pc";
              }
              else if (test4.equals("24")) {
                test4 = "Pd";
              }
              else if (test4.equals("25")) {
                test4 = "Pe";
              }
              else if (test4.equals("26")) {
                test4 = "Pf";
              }
              else if (test4.equals("27")) {
                test4 = "P1";
              }
              else if (test4.equals("28")) {
                test4 = "P2";
              }
              else if (test4.equals("29")) {
                test4 = "P3";
              }
              else if (test4.equals("41")) {
                test4 = "Qa";
              }
              else if (test4.equals("42")) {
                test4 = "Qb";
              }
              else if (test4.equals("43")) {
                test4 = "Qc";
              }
              else if (test4.equals("44")) {
                test4 = "Qd";
              }
              else if (test4.equals("45")) {
                test4 = "Qe";
              }
              else if (test4.equals("46")) {
                test4 = "Qf";
              }
              else if (test4.equals("47")) {
                test4 = "Q1";
              }
              else if (test4.equals("48")) {
                test4 = "Q2";
              }
              else if (test4.equals("49")) {
                test4 = "Q3";
              }
              else if (test4.equals("40")) {
                test4 = "Q";
              }
              else if (test4.equals("60")) {
                test4 = "S";
              }
              else if (test4.equals("61")) {
                test4 = "Cs";
              }
              //新加类型
              else if (test4.equals("62")) {
                test4 = "I1";
              }
              else if (test4.equals("63")) {
                test4 = "I2";
              }
              else if (test4.equals("64")) {
                test4 = "I3";
              }
              else if (test4.equals("65")) {
                test4 = "Ia1";
              }
              else if (test4.equals("66")) {
                test4 = "Ia2";
              }
              else if (test4.equals("67")) {
                test4 = "Ia3";
              }
              else if (test4.equals("68")) {
                test4 = "Ib1";
              }
              else if (test4.equals("69")) {
                test4 = "Ib2";
              }
              else if (test4.equals("70")) {
               test4 = "Ib3";
             }
             else if (test4.equals("71")) {
               test4 = "Ic1";
             }
             else if (test4.equals("72")) {
               test4 = "Ic2";
             }
             else if (test4.equals("73")) {
              test4 = "Ic3";
             }
             else if (test4.equals("74")) {
               test4 = "P1";
             }
             else if (test4.equals("75")) {
               test4 = "P2";
             }
             else if (test4.equals("76")) {
               test4 = "P3";
             }
             else if (test4.equals("77")) {
               test4 = "Pa1";
             }
             else if (test4.equals("78")) {
               test4 = "Pa2";
             }
             else if (test4.equals("79")) {
               test4 = "Pa3";
             }
             else if (test4.equals("80")) {
               test4 = "Pb1";
             }
             else if (test4.equals("81")) {
               test4 = "Pb2";
             }
             else if (test4.equals("82")) {
               test4 = "Pb3";
             }
             else if (test4.equals("83")) {
               test4 = "Pc1";
             }
             else if (test4.equals("84")) {
               test4 = "Pc2";
             }
             else if (test4.equals("85")) {
               test4 = "Pc3";
             }
             else if (test4.equals("86")) {
               test4 = "Q1";
             }
             else if (test4.equals("87")) {
               test4 = "Q2";
             }
             else if (test4.equals("88")) {
               test4 = "Q3";
             }
             else if (test4.equals("89")) {
               test4 = "Qa1";
             }
             else if (test4.equals("90")) {
               test4 = "Qa2";
             }
             else if (test4.equals("91")) {
               test4 = "Qa3";
             }
             else if (test4.equals("92")) {
               test4 = "Qb1";
             }
             else if (test4.equals("93")) {
               test4 = "Qb2";
             }
             else if (test4.equals("94")) {
               test4 = "Qb3";
             }
             else if (test4.equals("95")) {
               test4 = "Qc1";
             }
             else if (test4.equals("96")) {
               test4 = "Qc2";
             }
             else if (test4.equals("97")) {
               test4 = "Qc3";
             }
             else if (test4.equals("101")) {
                test4 = "Pl";
              }
              else if (test4.equals("102")) {
                test4 = "Wd";
              }
              else if (test4.equals("103")) {
                test4 = "Yl";
              }
              else if (test4.equals("104")) {
                test4 = "Sw";
              }
              else if (test4.equals("105")) {
                test4 = "Ll";
              }
              else if (test4.equals("106")) {
                test4 = "Sd";
              }
              else if (test4.equals("107")) {
                test4 = "Dw";
              }
              else if (test4.equals("108")) {
                test4 = "T0";
              }
              else if (test4.equals("109")) {
                test4 = "T1";
              }
              else if (test4.equals("110")) {
                test4 = "T2";
              }
              else if (test4.equals("127")) {
                test4 = "Yc";
              }
              else if (test4.equals("150")) {
                test4 = "V";
              }
              else if (test4.equals("151")) {
                test4 = "Va";
              }
              else if (test4.equals("152")) {
                test4 = "Vb";
              }
              else if (test4.equals("153")) {
                test4 = "Vc";
              }
              else if (test4.equals("154")) {
                test4 = "Vd";
              }
              else if (test4.equals("155")) {
                test4 = "Ve";
              }
              else if (test4.equals("156")) {
                test4 = "Vf";
              }
              else if (test4.equals("157")) {
                test4 = "V0";
              }
              else if (test4.equals("158")) {
                test4 = "V1";
              }
              else if (test4.equals("159")) {
                test4 = "V2";
              }
              else if (test4.equals("200")) {
                test4 = "Wh";
              }
              else if (test4.equals("201")) {
                test4 = "Vh";
              }
              else if (test4.equals("202")) {
                test4 = "Wp";
              }
              else if (test4.equals("203")) {
                test4 = "Wn";
              }
              else if (test4.equals("204")) {
                test4 = "Vp";
              }
              else if (test4.equals("205")) {
                test4 = "Vn";
              }
              else if (test4.equals("206")) {
                test4 = "Rt";
              }
              else if ( (test4.equals("300")) || (test4.equals("301")) ||
                       (test4.equals("302")) || (test4.equals("303")) ||
                       (test4.equals("304") || (test4.equals("305")))) {
                test4 = "Kg";
              }
              else if (test4.equals("400")) {
                test4 = "Zt";
              }
              else if ( (test4.equals("500")) || (test4.equals("501")) ||
                       (test4.equals("502")) || (test4.equals("503")) ||
                       (test4.equals("504")) || (test4.equals("505")) ||
                       (test4.equals("506")) || (test4.equals("507")) ||
                       (test4.equals("508")) || (test4.equals("509")) ||
                       (test4.equals("510")) || (test4.equals("541")) ||
                       (test4.equals("542")) || (test4.equals("543")) ||
                       (test4.equals("544")) || (test4.equals("571")) ||
                       (test4.equals("572") || (test4.equals("500")))) {
                test4 = "Dz";
              }
              else if (test4.equals("600")) {
                test4 = "Bh";
              }
              else if (test4.equals("601")) {
                test4 = "Sg";
              }
              else if (test4.equals("602")) {
                test4 = "Yg";
              }
              else if (test4.equals("603")) {
                  test4 = "Bh";
              }
              else if (test4.equals("604")) {
                  test4 = "Bh";
              }
              else if (test4.equals("605")) {
                  test4 = "Bh";
              }
              else if (test4.equals("606")) {
                  test4 = "Bh";
              }
              else if (test4.equals("607")) {
                  test4 = "Bh";
              }
              else if (test4.equals("608")) {
                  test4 = "Bh";
              }
              else if ( (test4.equals("4001")) || (test4.equals("4002")) ||
                       (test4.equals("4003")) || (test4.equals("4004")) ||
                       (test4.equals("4005")) || (test4.equals("4011")) ||
                       (test4.equals("4012")) || (test4.equals("4013")) ||
                       (test4.equals("4014")) || (test4.equals("4015")) ||
                       (test4.equals("4016")) || (test4.equals("4017")) ||
                       (test4.equals("4018")) || (test4.equals("4019")) ||
                       (test4.equals("4020")) || (test4.equals("4021")) ||
                       (test4.equals("4020")) || (test4.equals("4021")) ||
                       (test4.equals("4033")) || (test4.equals("4034")) ||
                       (test4.equals("4035")) || (test4.equals("4036")) ||
                       (test4.equals("4031") || (test4.equals("4032")) ||
                        (test4.equals("4037")))) {
                test4 = "Da";
              }
              else if ( (test4.equals("4101")) || (test4.equals("4102")) ||
                       (test4.equals("4103")) || (test4.equals("4104")) ||
                       (test4.equals("4105")) || (test4.equals("4111")) ||
                       (test4.equals("4112")) || (test4.equals("4113")) ||
                       (test4.equals("4114")) || (test4.equals("4115")) ||
                       (test4.equals("4121")) || (test4.equals("4122")) ||
                       (test4.equals("4123")) || (test4.equals("4124")) ||
                       (test4.equals("4125")) || (test4.equals("4126")) ||
                       (test4.equals("4127")) || (test4.equals("4128")) ||
                       (test4.equals("4129")) || (test4.equals("4130")) ||
                       (test4.equals("4131")) || (test4.equals("4132")) ||
                       (test4.equals("4141")) || (test4.equals("4142")) ||
                       (test4.equals("4141")) || (test4.equals("4142")) ||
                       (test4.equals("4143")) || (test4.equals("4144")) ||
                       (test4.equals("4145")) || (test4.equals("4146")) ||
                       (test4.equals("4147")) || (test4.equals("4148")) ||
                       (test4.equals("4149")) || (test4.equals("4150")) ||
                       (test4.equals("4151"))) {
                test4 = "Ftu";
              }
              if (PreProcessor.ALIAS) {
                if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                    "代码")) {
                  t0 = j;
                  if (oo == null) {
                    colvalue[coltoallcol[t0]] = (test1 + test3 + test4).
                        getBytes();
                    if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).
                        equalsIgnoreCase(
                            "代码")) {
                      //20080901新疆系统需要602、400类型加遥信号修改如下代码
                      if (test4.equalsIgnoreCase("Bh")) {
                        tablemodeltmp.setValueAt(test1 + test3 + test4, i,
                                                 keycolno[0]);
                      }
                      else if (test4.equalsIgnoreCase("Yj")
                              || test4.equalsIgnoreCase("Cd")
                              || test4.equalsIgnoreCase("Cn")
                              || test4.equalsIgnoreCase("Yc")
                              ) {
                          tablemodeltmp.setValueAt(test1 + test3 + test4, i, keycolno[0]);
                      }
                      else if (PreProcessor.XINJIANG &&
                               (test4.equalsIgnoreCase("Yg") ||
                                test4.equalsIgnoreCase("Zt"))) {
                        tablemodeltmp.setValueAt(test1 + test3 + test4, i,
                                                 keycolno[0]);
                      }
                      else {
                        tablemodeltmp.setValueAt(test1 + test4, i, keycolno[0]);
                      }
                      /*if(test1.substring(0,1).equalsIgnoreCase("P")){
                       tablemodeltmp.setValueAt(test1 + test3 + test4,i,keycolno[0]);
                                                    }else{
                       tablemodeltmp.setValueAt(test1 + test4,i,keycolno[0]);
                                                    }*/
                    }
                  }
                } // end if 代码
              }
              else {
                if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                    "代码")) {
                  t0 = j;
                  if (oo == null) {
                    colvalue[coltoallcol[t0]] = (test1 + test3 + test4).
                        getBytes();
                    if (tablemodeltmp.getColumnName(coltabnono[j]).
                        equalsIgnoreCase(
                            "代码")) {
                      //20080901新疆系统需要602、400类型加遥信号修改如下代码
                      if (test4.equalsIgnoreCase("Bh")) {
                        tablemodeltmp.setValueAt(test1 + test3 + test4, i,
                                                 keycolno[0]);
                      }
                      else if (test4.equalsIgnoreCase("Yj")
                              || test4.equalsIgnoreCase("Cd")
                              || test4.equalsIgnoreCase("Cn")
                              || test4.equalsIgnoreCase("Yc")
                              ) {
                          tablemodeltmp.setValueAt(test1 + test3 + test4, i, keycolno[0]);
                      }
                      else if (PreProcessor.XINJIANG &&
                               (test4.equalsIgnoreCase("Yg") ||
                                test4.equalsIgnoreCase("Zt"))) {
                        tablemodeltmp.setValueAt(test1 + test3 + test4, i,
                                                 keycolno[0]);
                      }
                      else {
                        tablemodeltmp.setValueAt(test1 + test4, i, keycolno[0]);
                      }
                      /*if(test1.substring(0,1).equalsIgnoreCase("P")){
                       tablemodeltmp.setValueAt(test1 + test3 + test4,i,keycolno[0]);
                                                    }else{
                       tablemodeltmp.setValueAt(test1 + test4,i,keycolno[0]);
                                                    }*/
                    }
                  }
                } // end if 代码
              }
            }  // end if 设备代码、遥测、遥信、电度序号、已转换类型->都有值
          }
          if (oo != null) {
            if (oo instanceof Boolean) {
              boolean r = oo.toString().trim().equalsIgnoreCase("true");
              if (r) {
                colvalue[coltoallcol[j]] = "1".getBytes();
              }
              else {
                colvalue[coltoallcol[j]] = "0".getBytes();
              }
            }
            else {
              String fff ="";
              if(PreProcessor.ALIAS)
                 fff= GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j]));
              else
                 fff= tablemodeltmp.getColumnName(coltabnono[j]);

              colvalue[coltoallcol[j]] = oo.toString().trim().getBytes();
               if(PreProcessor.MD5){
                 // add by hx -20121204
                 if (tablename.equals("用户口令参数表")) {
                   if (tablemodeltmp.getColumnName(coltabnono[j]).
                       equalsIgnoreCase(
                           "密码")) {
                     test1 = oo.toString().trim();
                     Md5Str md5str = new Md5Str();
                     colvalue[coltoallcol[j]] = md5str.PWDtoMD5(test1).getBytes();
                   }
                 }
                 //add by hx -20121204
                 // add by hx -20121204
                 if (tablename.equals("web用户表")) {
                   if (tablemodeltmp.getColumnName(coltabnono[j]).
                       equalsIgnoreCase(
                           "口令")) {
                     test1 = oo.toString().trim();
                     Md5Str md5str = new Md5Str();
                     colvalue[coltoallcol[j]] = md5str.PWDtoMD5(test1).getBytes();
                   }
                 }
               }
              //add by hx -20121204
              if (tablename.equals("遥信参数表") || tablename.equals("遥测参数表") ||
                  tablename.equals("电度参数表")) {
                /*      buf = pdll.queryAnyTable(m_db,
                                               "SCD对象类型表",
                                               "类型值,类型代码", "", "",
                                               fieldcount, true);
                      len = buf.length;
                      if (len > 0) {
                        for (i = 0; i < len; i++) {
                          code = pdll.substr(buf[i]).trim();
                        }
                      }*/
                if (PreProcessor.ALIAS) {
                  if (GetRealColName(tablemodeltmp.getTableName(),
                                     tablemodeltmp.getColumnName(coltabnono[j])).
                      equalsIgnoreCase(
                          "设备代码")) {
                    test1 = oo.toString().trim();
                  }

                }
                else {
                  if (tablemodeltmp.getColumnName(coltabnono[j]).
                      equalsIgnoreCase(
                          "设备代码")) {
                    test1 = oo.toString().trim();
                  }
                }

                if (PreProcessor.ALIAS) {
                  if (GetRealColName(tablemodeltmp.getTableName(),
                                     tablemodeltmp.getColumnName(coltabnono[j])).
                      equalsIgnoreCase(
                          "终端序号")) {
                    test2 = oo.toString().trim();
                  }
                }
                else {
                  if (tablemodeltmp.getColumnName(coltabnono[j]).
                      equalsIgnoreCase(
                          "终端序号")) {
                    test2 = oo.toString().trim();
                  }
                }

                if (PreProcessor.ALIAS) {
                  if (GetRealColName(tablemodeltmp.getTableName(),
                                     tablemodeltmp.getColumnName(coltabnono[j])).
                      equalsIgnoreCase(
                          "遥信序号") ||
                      GetRealColName(tablemodeltmp.getTableName(),
                                     tablemodeltmp.getColumnName(coltabnono[j])).
                      equalsIgnoreCase(
                          "遥测序号") ||
                      GetRealColName(tablemodeltmp.getTableName(),
                                     tablemodeltmp.getColumnName(coltabnono[j])).
                      equalsIgnoreCase(
                          "电度序号")) {
                    test3 = oo.toString().trim();
                  }
                }
                else {
                  if (tablemodeltmp.getColumnName(coltabnono[j]).
                      equalsIgnoreCase(
                          "遥信序号") ||
                      tablemodeltmp.getColumnName(coltabnono[j]).
                      equalsIgnoreCase(
                          "遥测序号") ||
                      tablemodeltmp.getColumnName(coltabnono[j]).
                      equalsIgnoreCase(
                          "电度序号")) {
                    test3 = oo.toString().trim();
                  }
                }

                if (PreProcessor.ALIAS) {
                  if (GetRealColName(tablemodeltmp.getTableName(),
                                     tablemodeltmp.getColumnName(coltabnono[j])).
                      equalsIgnoreCase(
                          "类型")) {
                    test4 = oo.toString().trim();
                  }
                }
                else {
                  if (tablemodeltmp.getColumnName(coltabnono[j]).
                      equalsIgnoreCase(
                          "类型")) {
                    test4 = oo.toString().trim();
                  }
                }

                boolean bIsCode = false;
                if (PreProcessor.ALIAS) {
                  if (GetRealColName(tablemodeltmp.getTableName(),
                                     tablemodeltmp.getColumnName(coltabnono[j])).
                      equalsIgnoreCase("代码"))
                    bIsCode = true;
                }
                else {
                  if (tablemodeltmp.getColumnName(coltabnono[j]).
                      equalsIgnoreCase("代码"))
                    bIsCode = true;
                }
                if (bIsCode) {
                  t = j;
                  // ReadDB readdb = new ReadDB(ConstantDef.MODELDB, "SCD对象类型表");
                  //  statment = "类型值=" + Integer.parseInt(test4);
                  //  readdb.queryTable("类型代码",statment, "");
                  //  code = readdb.getAFieldValue( 0,0) ;
                  //  buf = pdll.queryAnyTable(m_db,
                  //   "SCD对象类型表",
                  //  "类型代码", statment, "",
                  //   fieldcount, false);
                  //if ( len > 0) {
                  //for (i = 0; i < len; i++) {
                  // code = pdll.substr(buf[0]).trim();
                  // len = buf.length;
                  //  }
//  }
                  if ( (test1 != null) && (test3 != null) &&
                      (test4 != null)) {
                    if (test4.equals("0")) {
                      test4 = "Dz1";
                    }
                    else if (test4.equals("1")) {
                      test4 = "I";
                    }
                    else if (test4.equals("2")) {
                      test4 = "Ia";
                    }
                    else if (test4.equals("3")) {
                      test4 = "Ib";
                    }
                    else if (test4.equals("4")) {
                      test4 = "Ic";
                    }
                    else if (test4.equals("5")) {
                      test4 = "Id";
                    }
                    else if (test4.equals("6")) {
                      test4 = "Ie";
                    }
                    else if (test4.equals("7")) {
                      test4 = "If";
                    }
                    else if (test4.equals("8")) {
                      test4 = "I0";
                    }
                    else if (test4.equals("9")) {
                      test4 = "I1";
                    }
                    else if (test4.equals("10")) {
                      test4 = "I2";
                    }
                    else if (test4.equals("20")) {
                      test4 = "P";
                    }
                    else if (test4.equals("21")) {
                      test4 = "Pa";
                    }
                    else if (test4.equals("22")) {
                      test4 = "Pb";
                    }
                    else if (test4.equals("23")) {
                      test4 = "Pc";
                    }
                    else if (test4.equals("24")) {
                      test4 = "Pd";
                    }
                    else if (test4.equals("25")) {
                      test4 = "Pe";
                    }
                    else if (test4.equals("26")) {
                      test4 = "Pf";
                    }
                    else if (test4.equals("40")) {
                      test4 = "Q";
                    }
                    else if (test4.equals("41")) {
                      test4 = "Qa";
                    }
                    else if (test4.equals("42")) {
                      test4 = "Qb";
                    }
                    else if (test4.equals("43")) {
                      test4 = "Qc";
                    }
                    else if (test4.equals("44")) {
                      test4 = "Qd";
                    }
                    else if (test4.equals("45")) {
                      test4 = "Qe";
                    }
                    else if (test4.equals("46")) {
                      test4 = "Qf";
                    }
                    else if (test4.equals("60")) {
                      test4 = "S";
                    }
                    else if (test4.equals("61")) {
                      test4 = "Cs";
                    }
                    //新加类型
                    else if (test4.equals("62")) {
                      test4 = "I1";
                    }
                    else if (test4.equals("63")) {
                      test4 = "I2";
                    }
                    else if (test4.equals("64")) {
                      test4 = "I3";
                    }
                    else if (test4.equals("65")) {
                      test4 = "Ia1";
                    }
                    else if (test4.equals("66")) {
                      test4 = "Ia2";
                    }
                    else if (test4.equals("67")) {
                      test4 = "Ia3";
                    }
                    else if (test4.equals("68")) {
                      test4 = "Ib1";
                    }
                    else if (test4.equals("69")) {
                      test4 = "Ib2";
                    }
                    else if (test4.equals("70")) {
                      test4 = "Ib3";
                    }
                    else if (test4.equals("71")) {
                      test4 = "Ic1";
                    }
                    else if (test4.equals("72")) {
                      test4 = "Ic2";
                    }
                    else if (test4.equals("73")) {
                      test4 = "Ic3";
                    }
                    else if (test4.equals("74")) {
                      test4 = "P1";
                    }
                    else if (test4.equals("75")) {
                      test4 = "P2";
                    }
                    else if (test4.equals("76")) {
                      test4 = "P3";
                    }
                    else if (test4.equals("77")) {
                      test4 = "Pa1";
                    }
                    else if (test4.equals("78")) {
                      test4 = "Pa2";
                    }
                    else if (test4.equals("79")) {
                      test4 = "Pa3";
                    }
                    else if (test4.equals("80")) {
                      test4 = "Pb1";
                    }
                    else if (test4.equals("81")) {
                      test4 = "Pb2";
                    }
                    else if (test4.equals("82")) {
                      test4 = "Pb3";
                    }
                    else if (test4.equals("83")) {
                      test4 = "Pc1";
                    }
                    else if (test4.equals("84")) {
                      test4 = "Pc2";
                    }
                    else if (test4.equals("85")) {
                      test4 = "Pc3";
                    }
                    else if (test4.equals("86")) {
                      test4 = "Q1";
                    }
                    else if (test4.equals("87")) {
                      test4 = "Q2";
                    }
                    else if (test4.equals("88")) {
                      test4 = "Q3";
                    }
                    else if (test4.equals("89")) {
                      test4 = "Qa1";
                    }
                    else if (test4.equals("90")) {
                      test4 = "Qa2";
                    }
                    else if (test4.equals("91")) {
                      test4 = "Qa3";
                    }
                    else if (test4.equals("92")) {
                      test4 = "Qb1";
                    }
                    else if (test4.equals("93")) {
                      test4 = "Qb2";
                    }
                    else if (test4.equals("94")) {
                      test4 = "Qb3";
                    }
                    else if (test4.equals("95")) {
                      test4 = "Qc1";
                    }
                    else if (test4.equals("96")) {
                      test4 = "Qc2";
                    }
                    else if (test4.equals("97")) {
                      test4 = "Qc3";
                    }
                    else if (test4.equals("101")) {
                      test4 = "Pl";
                    }
                    else if (test4.equals("102")) {
                      test4 = "Wd";
                    }
                    else if (test4.equals("107")) {
                      test4 = "Dw";
                    }
                    else if (test4.equals("127")) {
                      test4 = "Yc";
                    }
                    else if (test4.equals("150")) {
                      test4 = "V";
                    }
                    else if (test4.equals("151")) {
                      test4 = "Va";
                    }
                    else if (test4.equals("152")) {
                      test4 = "Vb";
                    }
                    else if (test4.equals("153")) {
                      test4 = "Vc";
                    }
                    else if (test4.equals("154")) {
                      test4 = "Vd";
                    }
                    else if (test4.equals("155")) {
                      test4 = "Ve";
                    }
                    else if (test4.equals("156")) {
                      test4 = "Vf";
                    }
                    else if (test4.equals("157")) {
                      test4 = "V0";
                    }
                    else if (test4.equals("158")) {
                      test4 = "V1";
                    }
                    else if (test4.equals("159")) {
                      test4 = "V2";
                    }
                    else if (test4.equals("200")) {
                      test4 = "Wh";
                    }
                    else if (test4.equals("201")) {
                      test4 = "Vh";
                    }
                    else if (test4.equals("202")) {
                      test4 = "Wp";
                    }
                    else if (test4.equals("203")) {
                      test4 = "Wn";
                    }
                    else if (test4.equals("204")) {
                      test4 = "Vp";
                    }
                    else if (test4.equals("205")) {
                      test4 = "Vn";
                    }
                    else if (test4.equals("206")) {
                      test4 = "Rt";
                    }
                    else if ( (test4.equals("300")) || (test4.equals("301")) ||
                             (test4.equals("302")) || (test4.equals("303")) ||
                             (test4.equals("304") || (test4.equals("305")))) {
                      test4 = "Kg";
                    }
                    else if (test4.equals("400")) {
                      test4 = "Zt";
                    }
                    else if ( (test4.equals("500")) || (test4.equals("501")) ||
                             (test4.equals("502")) || (test4.equals("503")) ||
                             (test4.equals("504")) || (test4.equals("505")) ||
                             (test4.equals("506")) || (test4.equals("507")) ||
                             (test4.equals("508")) || (test4.equals("509")) ||
                             (test4.equals("510")) || (test4.equals("541")) ||
                             (test4.equals("542")) || (test4.equals("543")) ||
                             (test4.equals("544")) || (test4.equals("571")) ||
                             (test4.equals("572") || (test4.equals("500")))) {
                      test4 = "Dz";
                    }
                    else if (test4.equals("600")) {
                      test4 = "Bh";
                    }
                    else if (test4.equals("601")) {
                      test4 = "Sg";
                    }
                    else if (test4.equals("602")) {
                      test4 = "Yg";
                    }
                    else if (test4.equals("603")) {
                        test4 = "Bh";
                    }
                    else if (test4.equals("604")) {
                        test4 = "Bh";
                    }
                    else if (test4.equals("605")) {
                        test4 = "Bh";
                    }
                    else if (test4.equals("606")) {
                        test4 = "Bh";
                    }
                    else if (test4.equals("607")) {
                        test4 = "Bh";
                    }
                    else if (test4.equals("608")) {
                        test4 = "Bh";
                    }
                    else if ( (test4.equals("4001")) || (test4.equals("4002")) ||
                             (test4.equals("4003")) || (test4.equals("4004")) ||
                             (test4.equals("4005")) || (test4.equals("4011")) ||
                             (test4.equals("4012")) || (test4.equals("4013")) ||
                             (test4.equals("4014")) || (test4.equals("4015")) ||
                             (test4.equals("4016")) || (test4.equals("4017")) ||
                             (test4.equals("4018")) || (test4.equals("4019")) ||
                             (test4.equals("4020")) || (test4.equals("4021")) ||
                             (test4.equals("4020")) || (test4.equals("4021")) ||
                             (test4.equals("4033")) || (test4.equals("4034")) ||
                             (test4.equals("4035")) || (test4.equals("4036")) ||
                             (test4.equals("4031") || (test4.equals("4032")) ||
                              (test4.equals("4037")))) {
                      test4 = "Da";
                    }
                    else if ( (test4.equals("4101")) || (test4.equals("4102")) ||
                             (test4.equals("4103")) || (test4.equals("4104")) ||
                             (test4.equals("4105")) || (test4.equals("4111")) ||
                             (test4.equals("4112")) || (test4.equals("4113")) ||
                             (test4.equals("4114")) || (test4.equals("4115")) ||
                             (test4.equals("4121")) || (test4.equals("4122")) ||
                             (test4.equals("4123")) || (test4.equals("4124")) ||
                             (test4.equals("4125")) || (test4.equals("4126")) ||
                             (test4.equals("4127")) || (test4.equals("4128")) ||
                             (test4.equals("4129")) || (test4.equals("4130")) ||
                             (test4.equals("4131")) || (test4.equals("4132")) ||
                             (test4.equals("4141")) || (test4.equals("4142")) ||
                             (test4.equals("4141")) || (test4.equals("4142")) ||
                             (test4.equals("4143")) || (test4.equals("4144")) ||
                             (test4.equals("4145")) || (test4.equals("4146")) ||
                             (test4.equals("4147")) || (test4.equals("4148")) ||
                             (test4.equals("4149")) || (test4.equals("4150")) ||
                             (test4.equals("4151"))) {
                      test4 = "Ftu";
                    }
                    //    tablemodeltmp.setValueAt(test1 + test3 + test4,0,t);
                    // colvalue[coltoallcol[t]] = (test1 + test3 + test4).getBytes();
                    bIsCode=false;
                    if (PreProcessor.ALIAS) {
                      if (GetRealColName(tablemodeltmp.getTableName(),
                                         tablemodeltmp.getColumnName(coltabnono[
                          j])).
                          equalsIgnoreCase(
                              "代码")) {
                        bIsCode = true;
                      }

                    }
                    else {
                      if (tablemodeltmp.getColumnName(coltabnono[j]).
                          equalsIgnoreCase(
                              "代码")) {
                        bIsCode = true;
                      }
                    }

                    if (bIsCode) {
                      // if (test1.substring(0,1).equalsIgnoreCase("P")){
                      //20080901新疆系统需要602、400类型加遥信号修改如下代码
                      if (test4.equalsIgnoreCase("Bh")) {
                        colvalue[coltoallcol[t]] = (test1 + test3 + test4).
                            getBytes();
                        tablemodeltmp.setValueAt(test1 + test3 + test4, i,
                                                 keycolno[0]);
                      }
                      else if (test4.equalsIgnoreCase("Yj")
                              || test4.equalsIgnoreCase("Cd")
                              || test4.equalsIgnoreCase("Cn")
                              || test4.equalsIgnoreCase("Yc")
                              ) {
                          colvalue[coltoallcol[t]] = (test1 + test3 + test4).
                                  getBytes();
                          tablemodeltmp.setValueAt(test1 + test3 + test4, i,
                                  keycolno[0]);
                      }
                      else if (PreProcessor.XINJIANG &&
                               (test4.equalsIgnoreCase("Yg") ||
                                test4.equalsIgnoreCase("Zt"))) {
                        colvalue[coltoallcol[t]] = (test1 + test3 + test4).
                            getBytes();
                        tablemodeltmp.setValueAt(test1 + test3 + test4, i,
                                                 keycolno[0]);
                      }
                      else {
                        colvalue[coltoallcol[t]] = (test1 + test4).getBytes();
                        tablemodeltmp.setValueAt(test1 + test4, i, keycolno[0]);
                      }
                    }
                  }
                }
              }
              /*  if (tablename.equals("终端参数表")) {
               if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                       "代码")) {
                     t = j;
                   }
               if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                       "站所线系代码")) {
                     test1 = oo.toString().trim();
                   }
                   //  colvalue[coltoallcol[t]] = (dateFormat.format(new Date())).getBytes();
                   if (test1 != null) {
                     colvalue[coltoallcol[t]] = test1.getBytes();
                     int bbbdds=0;
                   }
                 }*/


              if (tablename.equals("站所线系参数表")) {
                if (PreProcessor.ALIAS) {
                  if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).
                      equalsIgnoreCase(
                          "代码")) {
                    test1 = oo.toString().trim();
                    t = j;
                    if (test1 == null) {
                      //  colvalue[coltoallcol[t]] = (dateFormat.format(new Date())).getBytes();
                      random = (int) (Math.random() * 26);
                      random1 = (int) (Math.random() * 27);
                      random2 = (int) (Math.random() * 28);
                      random3 = (int) (Math.random() * 29);
                      char[] test = new char[4];
                      test[0] = letter[random];
                      test[1] = letter[random1];
                      test[2] = letter[random2];
                      test[3] = letter[random3];
                      colvalue[coltoallcol[t]] = String.valueOf(test).getBytes();
                    }
                  }
                }
                else {
                  if (tablemodeltmp.getColumnName(coltabnono[j]).
                      equalsIgnoreCase(
                          "代码")) {
                    test1 = oo.toString().trim();
                    t = j;
                    if (test1 == null) {
                      //  colvalue[coltoallcol[t]] = (dateFormat.format(new Date())).getBytes();
                      random = (int) (Math.random() * 26);
                      random1 = (int) (Math.random() * 27);
                      random2 = (int) (Math.random() * 28);
                      random3 = (int) (Math.random() * 29);
                      char[] test = new char[4];
                      test[0] = letter[random];
                      test[1] = letter[random1];
                      test[2] = letter[random2];
                      test[3] = letter[random3];
                      colvalue[coltoallcol[t]] = String.valueOf(test).getBytes();
                    }
                  }
                }
              }

              if (tablename.equals("SCD设备参数表")) { //复制SCD设备生成代码
                  if (oo != null) {
                      if (PreProcessor.ALIAS) {
                          if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                                  "类型")) {
                              test1 = tmp.elementAt(coltabnono[j]).toString().trim();
                          }
                      }
                      else {
                          if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                                  "类型")) {
                              test1 = tmp.elementAt(coltabnono[j]).toString().trim();
                          }
                      }

                      if (PreProcessor.ALIAS) {
                          if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                                  "站所线系代码")) {
                              test2 = tmp.elementAt(coltabnono[j]).toString().trim();
                          }
                      }
                      else {
                          if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                                  "站所线系代码")) {
                              test2 = tmp.elementAt(coltabnono[j]).toString().trim();
                          }
                      }

                      random = (int) (Math.random() * 32);
                      random1 = (int) (Math.random() * 34);
                      random2 = (int) (Math.random() * 29);
                      char[] test = new char[3];
                      test[0] = letter[random];
                      test[1] = letter[random1];
                      test[2] = letter[random2];
                      if (test1 != null && test2 != null) {
                          // if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                          //  "代码")) {
                          if (test1.equals("0")) {
                              test1 = "";
                          }
                          else if (test1.equals("1")) {
                              test1 = "K";
                          }
                          else if (test1.equals("2")) {
                              test1 = "D";
                          }
                          else if (test1.equals("3")) {
                              test1 = "";
                          }
                          else if (test1.equals("4")) {
                              test1 = "K";
                          }
                          else if (test1.equals("5")) {
                              test1 = "B";
                          }
                          else if (test1.equals("6")) {
                              test1 = "b";
                          }
                          else if (test1.equals("7")) {
                              test1 = "C";
                          }
                          else if (test1.equals("8")) {
                              test1 = "R";
                          }
                          else if (test1.equals("9")) {
                              test1 = "Q";
                          }
                          else if (test1.equals("10")) {
                              test1 = "U";
                          }
                          else if (test1.equals("11")) {
                              test1 = "I";
                          }
                          else if (test1.equals("12")) {
                              test1 = "L";
                          }
                          else if (test1.equals("13")) {
                              test1 = "D";
                          }
                          else if (test1.equals("14")) {
                              test1 = "X";
                          }
                          else if (test1.equals("15")) {
                              test1 = "x";
                          }
                          else if (test1.equals("16")) {
                              test1 = "M";
                          }
                          else if (test1.equals("17")) {
                              test1 = "F";
                          }
                          else if (test1.equals("18")) {
                              test1 = "J";
                          }
                          else if (test1.equals("19")) {
                              test1 = "J";
                          }
                          else if (test1.equals("20")) {
                              test1 = "J";
                          }
                          else if (test1.equals("21")) {
                              test1 = "H";
                          }
                          else if (test1.equals("22")) {
                              test1 = "P";
                          }
                          else if (test1.equals("23")) {
                              test1 = "K";
                          }
                          else if (test1.equals("25")) {
                              test1 = "O";
                          }
                          else if (test1.equals("26")) {
                              test1 = "K";
                          }
                          else if (test1.equals("27")) {
                              test1 = "K";
                          }
                          else if (test1.equals("28")) {
                              test1 = "K";
                          }
                          else if (test1.equals("29")) {
                              test1 = "O";
                          }
                          else if (test1.equals("30")) {
                              test1 = "O";
                          }
                          else if (test1.equals("51")) {
                              test1 = "O";
                          }
                          else if (test1.equals("52")) {
                              test1 = "S";
                          }
                          if (PreProcessor.ALIAS) {
                              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).
                                      equalsIgnoreCase(
                                              "代码")) {
                                  t = j;
                                  colvalue[coltoallcol[t]] = (test1 + test2 + String.valueOf(test)).getBytes();
                                  tablemodeltmp.setValueAt(test1 + test2 + String.valueOf(test), i, keycolno[0]);
                              }
                          }
                          else {
                              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase("代码")) {
                                  t = j;
                                  colvalue[coltoallcol[t]] = (test1 + test2 + String.valueOf(test)).getBytes();
                                  tablemodeltmp.setValueAt(test1 + test2 + String.valueOf(test), i, keycolno[0]);
                              }
                          }
                          //复制SCD设备参数时生成代码
                          colvalue[coltoallcol[t]] = (test1+test2+String.valueOf(test)).getBytes();
                          tablemodeltmp.setValueAt(test1+test2+String.valueOf(test),i,keycolno[0]);
                      }
                  }
              }

             //SCD设备参数表的devid自动赋值
             if (bisscddevtable) {
               if (PreProcessor.ALIAS) {
                 if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).
                     equalsIgnoreCase(
                         ConstantDef.SCDDEVID)) {
                   if (!PreProcessor.JINHUA)
                     maxdevid++;
                   colvalue[coltoallcol[j]] = Integer.toString(maxdevid).
                       getBytes();
                   tablemodeltmp.setValueAt(Integer.toString(maxdevid), i,
                                            coltoallcol[j]);
                   bdevidhas = true;
                 }
                 else if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).
                          equalsIgnoreCase(
                              ConstantDef.SCDTYPE)) {
                   devtype = Integer.parseInt(oo.toString());
                 }

               }
               else {
                 if (tablemodeltmp.getColumnName(coltabnono[j]).
                     equalsIgnoreCase(
                         ConstantDef.SCDDEVID)) {
                   if (!PreProcessor.JINHUA)
                     maxdevid++;
                   colvalue[coltoallcol[j]] = Integer.toString(maxdevid).
                       getBytes();
                   tablemodeltmp.setValueAt(Integer.toString(maxdevid), i,
                                            coltoallcol[j]);
                   bdevidhas = true;
                 }
                 else if (tablemodeltmp.getColumnName(coltabnono[j]).
                          equalsIgnoreCase(
                              ConstantDef.SCDTYPE)) {
                   devtype = Integer.parseInt(oo.toString());
                 }
               }
             }
            } //end else
            if (PreProcessor.ALIAS) {
              if (GetRealColName(tablemodeltmp.getTableName(),tablemodeltmp.getColumnName(coltabnono[j])).equalsIgnoreCase(
                  tablemodeltmp.getListField())) {
                sListFieldValue = oo.toString().trim();
                bListFieldF = true;
              }

            }
            else {
              if (tablemodeltmp.getColumnName(coltabnono[j]).equalsIgnoreCase(
                  tablemodeltmp.getListField())) {
                sListFieldValue = oo.toString().trim();
                bListFieldF = true;
              }

            }

          } //end if (oo != null) {
        }//end for each table row count
        /* if (tablename.equals("遥信参数表") || tablename.equals("遥测参数表") ||
             tablename.equals("电度参数表") || tablename.equals("SCD设备参数表") ||
             tablename.equals("站所线系参数表")) {
           if ( (test1 != null) && (test2 != null) && (test3 != null) &&
               (test4 != null)) {
             rtn = pdll.insertARecord(colvalue);
           }
         }*/
        //else{

        if (!bdevidhas && bisscddevtable) { //隐含了DEVID列
           if (!PreProcessor.JINHUA)
             maxdevid++;
          //devid 在第二列
          colvalue[ConstantDef.SCDDEVIDCOLNO] = Integer.toString(maxdevid).
              getBytes();
        }

        if(PreProcessor.ALIAS){ //因为别名修改的缘故，添加这部分防止发生更新表出错的情况。
           tb = tablename.trim().getBytes();
           rtn = pdll.openTableInfo(dbname.getBytes(), tb);
           if (rtn <= 0) {
             pdll.closeTableInfo();
             return false;
           }
         }

         //新增记录保存--南网测试
        String hostname = "";
        try {
          InetAddress neta = InetAddress.getLocalHost();
          hostname = neta.getHostName();
        }
        catch (UnknownHostException nete) {
          hostname = "";
        }

        User user = ConstantDef.getUserInstance();
        String dbglogstr = "插入记录  主机名： " + hostname + "  操作员： " + user.getDesc();
        dbglogstr += "    记录关键字：";
        //myTableModel1
        int keyCol[] = new int[vKey.size() + 1];
        for (int kk = 0; kk < keyCol.length - 1; kk++) {
          String tmpKey = String.valueOf(vKey.get(kk));
          dbglogstr += tmpKey;
          dbglogstr += "=";
          for (int ii = 0; ii < myTableModel1.columnIdentifiers.size(); ii++) {
            String tmpStr1 = String.valueOf(myTableModel1.columnIdentifiers.
                                            get(ii));
            if (tmpKey.compareToIgnoreCase(tmpStr1) == 0) {
              keyCol[kk] = ii;
              String strtmp2 = new String(colvalue[ii]);
              dbglogstr += strtmp2;
            }
          }
        }
        dbginfo.writelog(tablename, dbglogstr);


        rtn = pdll.insertARecord(colvalue);
        if (rtn <= 0) {
          vFailRow.add(Integer.toString(i));
          continue;
        } //insert 成功后
        else {
          if (bisscddevtable) {
            if(!PreProcessor.JINHUA)
              pdll.execproc(0, maxdevid, devtype, 0);
          }
          myTableModel1.rowProp.setElementAt(Integer.toString(ConstantDef.
              DB_UNCHANGED), i);
          myTableModel1.managemodel.setUnDoFlag(i);
          vdata = (Vector) myTableModel1.dataVector.elementAt(i);
          myTableModel1.dataVectorBk.setElementAt(vdata.clone(), i);
          if (bListFieldF) {
            myTableModel1.setListField(i, sListFieldValue);
            jtable1.updateRow();
          }

          if (curTabnoCount > 1) {
            for (j = 0; j < curTabnoCount - 1; j++) {
              MyJTable jtabtmp = jtable1.getJTable(j);
              MyTableModel mytb = (MyTableModel) jtabtmp.getModel();
              vdata = (Vector) mytb.dataVector.elementAt(i);
              mytb.dataVectorBk.setElementAt(vdata.clone(), i);
              mytb.rowProp.setElementAt(Integer.toString(ConstantDef.
                  DB_UNCHANGED), i);
              if (bListFieldF) {
                mytb.setListField(i, sListFieldValue);
              }
              jtabtmp.updateRow();
            } //for j
          } //else
        } //if(curTabnoCount == 1)
      } //if
    } //for i

    pdll.closeTableInfo();

    if (vFailRow.size() > 0) {
      // if(tablename.equalsIgnoreCase(ConstantDef.SCDDEVTABLE) )
      {
      //  pdll.closedb(); //当遇到有关键字重复而不能写成功时，执行select max(devid) from scd设备表  语句会死掉。
      //  pdll.startdb();
      }
      return false;
    }
    return true;
  }

  public void unRemove(int tabno) {
    if (tabno < 0) {
      return;
    }
    int curTabnoCount = Integer.parseInt(vTableTabnoCount.elementAt(
        tabno).toString().trim()); //表的分区数
    if (curTabnoCount <= 0) {
      return;
    }
    MyJTable jtable1 = (MyJTable) vListTable.elementAt(tabno);
    MyTableModel myTableModel1 = (MyTableModel) jtable1.getModel();
    int nums = myTableModel1.unRemove();
    myTableModel1.managemodel.unRemoveRecs(nums);
    jtable1.updateRow();

    if (jtable1.getTabCount() > 1) {
      for (int i = 0; i < jtable1.getTabCount() - 1; i++) {
        MyJTable jtabtmp = jtable1.getJTable(i);
        MyTableModel mytb = (MyTableModel) jtabtmp.getModel();
        mytb.unRemove();
        jtabtmp.updateRow();
      }
    }
  }

  public boolean removeATab(int tabindex, boolean bSaveFlag) {
    if (bSaveFlag && hintToSave(tabindex)) { //表需要保存
      int answer = JOptionPane.showConfirmDialog(null,
                                                 "表\"" +
                                                 jtab.getTitleAt(tabindex) +
                                                 "\"需要保存", "保存提示信息",
                                                 JOptionPane.
                                                 YES_NO_CANCEL_OPTION);
      if (answer == JOptionPane.CANCEL_OPTION) {
        return false;
      }
      else if (answer == JOptionPane.OK_OPTION) {
        int saveflag = saveChange(tabindex,
                                  jtab.getToolTipTextAt(tabindex).trim());
        if (saveflag <= 0) {
          JOptionPane.showMessageDialog(null,
                                        "表\"" + jtab.getTitleAt(tabindex) +
                                        "\"保存出错!", "保存提示信息",
                                        JOptionPane.ERROR_MESSAGE);
          return false;
        }
      }
   if (PreProcessor.TableLock) {
     //表互斥修改
      MyTableModel myTableModel1 = (MyTableModel) ( (MyJTable)
          vListTable.elementAt(tabindex)).
          getModel();
      String where = "";

      if (ConstantDef.Vtablename.contains(myTableModel1.getTableName())) {
        ConstantDef.Vtablename.removeElement(myTableModel1.getTableName());
        int rtn = pdll.openTableInfo(ConstantDef.MODELDB.getBytes(),
                                     "table_status".getBytes());
        if (rtn <= 0) {
          pdll.closeTableInfo();
        }
        else {
          where = "tablename = '" + myTableModel1.getTableName() + "'";
          byte[] wh = where.trim().getBytes();
          pdll.removeARecord(wh);
          pdll.closeTableInfo();
        }
      }

//表修改互斥
   }
    }


    vTabnoJTab.remove(tabindex);
    vTableTabnoCount.remove(tabindex);
    vListTable.remove(tabindex);
    jtab.remove(tabindex);
    vOpenLeafNode.remove(tabindex);
    vTitle.remove(tabindex);   //madyang
    this.updateUI();
    return true;
  }

//判断某一个表是否需要保存
  public boolean hintToSave(int tabno) {
    if (tabno >= 0) {
      int tabnocount = Integer.parseInt(vTableTabnoCount.elementAt(
          tabno).toString().trim()); //分区数
      MyJTable jtable1;
      jtable1 = (MyJTable) vListTable.elementAt(tabno);
      MyTableModel myTableModel = (MyTableModel) jtable1.getModel();
      if (myTableModel.getSaveFlag()) {
        return true;
      }
      for (int i = 0; i < jtable1.getTabCount() - 1; i++) {
        MyJTable jtabtmp = jtable1.getJTable(i);
        MyTableModel mytb = (MyTableModel) jtabtmp.getModel();
        if (mytb.getSaveFlag()) {
          return true;
        }
      }
    }
    return false;
  }

  private String GetRealColName(String table,String tmpCol){
    String colName=ApplicationDB.frameDB.GetFieldNameFromAlias(table,tmpCol);
    if(colName.length()==0)
      colName=tmpCol;
    return colName;
  }

}


