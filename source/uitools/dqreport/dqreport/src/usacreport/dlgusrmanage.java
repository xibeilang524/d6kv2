package usacreport;

import java.awt.*;
import javax.swing.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import java.io.File;
import java.util.*;
import javax.swing.border.*;
import com.borland.dbswing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeSelectionModel;
import java.beans.*;
import java.awt.BorderLayout;
import java.awt.Font;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 * 这部分是对webrptname表进行管理的对话框
 */
public class dlgusrmanage extends JDialog {
  private JPanel panel1 = new JPanel();
  private JLabel jLabel1 = new JLabel();
  private XYLayout xYLayout1 = new XYLayout();
  private JComboBox jComtype = new JComboBox();
  public static JList jListAll = new JList();
  private JList jListtype = new JList();
  private JButton jButAdd = new JButton();
  private JButton jButDel = new JButton();
  private JButton jButcan = new JButton();
  public static Vector vecfile = new Vector();
  private Vector vectorin = new Vector();
  private TitledBorder titledBorder1;
  private JScrollPane jScrollPane1 = new JScrollPane();
  private JScrollPane jScrollPane2 = new JScrollPane();
  private String dbname = "modeldb";
  private String tablename = "webRptName";
  private String fieldname = "RptName";
  private String where = "",order = "webID";
  private int nCount = 0;
  private JComboBox jComhour = new JComboBox();
  private JComboBox jComdir = new JComboBox();
  private JComboBox jComptype = new JComboBox();
  private JComboBox jCommin = new JComboBox();
  private JLabel jLabel2 = new JLabel();
  private JLabel jLabel3 = new JLabel();
  private JLabel jLabel4 = new JLabel();
  private JLabel jLabel5 = new JLabel();
  private JLabel jLabel6 = new JLabel();
  private JComboBox jComtypet = new JComboBox();
  private JLabel jLabel7 = new JLabel();
  private String tree_subid = "0";
  private JLabel jLabel8 = new JLabel();
   JLabel jLabel9 = new JLabel();
   JComboBox jComGroup = new JComboBox();
   JComboBox jComday = new JComboBox();
   JLabel jLabel10 = new JLabel();

   public dlgusrmanage(Frame frame, String title, boolean modal) {
    super(frame, title, modal);
    try {
      jbInit();
      pack();
    }
    catch(Exception ex) {
      ex.printStackTrace();
    }
  }

  public dlgusrmanage() {
    this(null, "", false);
  }
  private void jbInit() throws Exception {
    titledBorder1 = new TitledBorder("");
    panel1.setLayout(xYLayout1);
    panel1.setBorder(new EmptyBorder(15, 15, 15, 15));
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("报表类型");
    jButAdd.setFont(new java.awt.Font("Dialog", 0, 12));
    jButAdd.setDoubleBuffered(true);
    jButAdd.setSelected(true);
    jButAdd.setText("添加>>");
    jButAdd.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButAdd_actionPerformed(e);
      }
    });
    jButDel.setFont(new java.awt.Font("Dialog", 0, 12));
    jButDel.setDoubleBuffered(true);
    jButDel.setText("<<删除");
    jButDel.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButDel_actionPerformed(e);
      }
    });
    jButcan.setFont(new java.awt.Font("Dialog", 0, 12));
    jButcan.setDoubleBuffered(true);
    jButcan.setText("退出");
    jButcan.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jButcan_actionPerformed(e);
      }
    });
    jListAll.setToolTipText("");
    jListAll.setFont(new java.awt.Font("Dialog", 0, 12));
    jListAll.setAutoscrolls(true);
    jListAll.setDoubleBuffered(true);
    jListAll.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
    jListAll.addMouseListener(new java.awt.event.MouseAdapter() {
      public void mouseClicked(MouseEvent e) {
        if(e.getClickCount() == 2){
          OnButAdd();
        }
      }
    });
    jListtype.setFont(new java.awt.Font("Dialog", 0, 12));
    jListtype.setDoubleBuffered(true);
    jListtype.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
    jListtype.addMouseListener(new java.awt.event.MouseAdapter() {
      public void mouseClicked(MouseEvent e) {
        if(e.getClickCount() == 2){
          OnButDel();
        }
      }
    });
    this.setModal(true);
    this.setResizable(false);
    jComtype.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComtype_actionPerformed(e);
      }
    });
    jComtype.setFont(new java.awt.Font("Dialog", 0, 12));
    jComtype.setAutoscrolls(true);
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel2.setText("打印时间");
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setText("时");
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setText("分");
    jLabel5.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel5.setText("纸张类型");
    jLabel6.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel6.setText("纵向");
    jComtypet.setFont(new java.awt.Font("Dialog", 0, 12));
    jComtypet.setAutoscrolls(true);
    jComtypet.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        jComtypet_actionPerformed(e);
      }
    });
    jLabel7.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel7.setText("所属节点");
    jComhour.setFont(new java.awt.Font("Dialog", 0, 12));
    jCommin.setFont(new java.awt.Font("Dialog", 0, 12));
    jComptype.setFont(new java.awt.Font("Dialog", 0, 12));
    jComdir.setFont(new java.awt.Font("Dialog", 0, 12));
      jLabel9.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel9.setText("分组名称");
      jComGroup.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jComGroup.addItem("");
      jComGroup.setAutoscrolls(true);
      jComGroup.setEditable(true);
      jComday.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel10.setFont(new java.awt.Font("Dialog", Font.PLAIN, 12));
      jLabel10.setText("日");
      panel1.add(jLabel8, new XYConstraints(30, 295, 216, 37));
      panel1.add(jLabel5, new XYConstraints(328, 47, 72, 18));
      panel1.add(jLabel6, new XYConstraints(481, 43, 32, 27));
      panel1.add(jComptype, new XYConstraints(403, 42, 73, -1));
      panel1.add(jComdir, new XYConstraints(516, 41, 65, -1));
      panel1.add(jComGroup, new XYConstraints(403, 77, 178, -1));
      panel1.add(jComday, new XYConstraints(69, 72, 47, -1));
      panel1.add(jLabel10, new XYConstraints(123, 73, -1, -1));
      panel1.add(jComhour, new XYConstraints(137, 70, 47, -1));
      panel1.add(jLabel3, new XYConstraints(188, 73, 18, -1));
      panel1.add(jCommin, new XYConstraints(207, 71, 47, -1));
      panel1.add(jLabel4, new XYConstraints(262, 74, 18, -1));
      panel1.add(jButAdd, new XYConstraints(262, 126, 84, -1));
      panel1.add(jButDel, new XYConstraints(261, 238, 84, 26));
      panel1.add(jButcan, new XYConstraints(262, 353, 84, -1));
      panel1.add(jLabel2, new XYConstraints(10, 72, 62, 27));
      panel1.add(jScrollPane2, new XYConstraints(357, 110, 224, 307));
      panel1.add(jScrollPane1, new XYConstraints(9, 110, 225, 306));
      panel1.add(jLabel9, new XYConstraints(328, 79, -1, -1));
      panel1.add(jComtypet, new XYConstraints(403, 3, 177, -1));
      panel1.add(jLabel7, new XYConstraints(327, 0, 63, 31));
      panel1.add(jComtype, new XYConstraints(84, 3, 170, 23));
      panel1.add(jLabel1, new XYConstraints(9, 0, 63, 31));
      jScrollPane1.getViewport().add(jListAll);
      jScrollPane2.getViewport().add(jListtype);

      this.getContentPane().add(panel1, java.awt.BorderLayout.NORTH);
      try{
      jComtype.addItem("打印日报表");
      jComtype.addItem("打印月报表");
      jComtype.addItem("日报表");
      jComtype.addItem("月报表");
      jComtype.addItem("年报表");
      //-----hongxiang
      jComtype.addItem("时报表");
      jComtype.addItem("周报表");
      jComtype.addItem("季报表");
      //-----hongxiang
      jComtype.setSelectedIndex(0);

      int i;
      for(i=0;i<usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.size();i++){
        jComtypet.addItem(usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.get(i));
      }
      if(jComtypet.getItemCount()>0){
        tree_subid = (String)usacreport.FrmSacReport.dlgtreem1.vec_treenodeid.get(0);
        jComtypet.setSelectedIndex(0);
      }
      else{
        return;
      }
      for(i=1;i<31;i++)
      {
         jComday.addItem(Integer.toString(i));
      }
      jComday.setSelectedIndex(0);
      for(i=0;i<=24;i++){
        jComhour.addItem(Integer.toString(i));
      }
      jComhour.setSelectedIndex(0);
      for(i=0;i<=60;i++){
        jCommin.addItem(Integer.toString(i));
      }
      jCommin.setSelectedIndex(0);
      jComptype.addItem("A4");
      jComptype.addItem("A3");
      jComptype.setSelectedIndex(0);
      jComdir.addItem("是");
      jComdir.addItem("否");
      jComdir.setSelectedIndex(0);
    }
    catch(java.lang.IllegalArgumentException etype){
    }
    //填充分组名称列表
    FillGroupname();
    Freshlist();
  }

  void jButcan_actionPerformed(ActionEvent e) {
    this.hide();
  }

  void jButAdd_actionPerformed(ActionEvent e) {
    OnButAdd();
  }

  void jButDel_actionPerformed(ActionEvent e) {
    int pos = jListtype.getSelectedIndex();
    if(pos < 0 ){
      return;
    }
    String str = (String)jListtype.getSelectedValue();
    vecfile.addElement(str + ".vts");
    jListAll.setListData(vecfile);

    int i=0,j=0;
    for(i=0;i<vectorin.size();i+=10){
      if(vectorin.get(i).equals(str)){
        vectorin.remove(i);
        vectorin.remove(i);
        vectorin.remove(i);
        vectorin.remove(i);
        vectorin.remove(i);
        vectorin.remove(i);
        vectorin.remove(i);
        vectorin.remove(i);
        vectorin.remove(i);
        vectorin.remove(i);
        break;
      }
    }
    fieldname = "RptName";
    try
    {
      switch(jComtype.getSelectedIndex())
      {
        case 0:
          where = "RPTTYPE = '日报表' and RPTNAME = '" + str + "'" + " and TIMEPRINTFLAG = 1";
          break;
        case 1:
          where = "RPTTYPE = '月报表' and RPTNAME = '" + str + "'" + " and TIMEPRINTFLAG = 1";
          break;
        case 2:
          where = "RPTTYPE = '日报表' and RPTNAME = '" + str + "'" + " and TIMEPRINTFLAG = 0";
          break;
        case 3:
          where = "RPTTYPE = '月报表' and RPTNAME = '" + str + "'";
          break;
       case 4:
          where = "RPTTYPE = '年报表' and RPTNAME = '" + str + "'";
          break;
       case 5:
          where = "RPTTYPE = '时报表' and RPTNAME = '" + str + "'";
          break;
       case 6:
          where = "RPTTYPE = '周报表' and RPTNAME = '" + str + "'";
          break;
       case 7:
          where = "RPTTYPE = '季报表' and RPTNAME = '" + str + "'";
          break;



      }
    }
    catch(java.lang.IllegalArgumentException ejcom){
    }

    Freshtype();
    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());

    if(returnv > 0){
      int m_ret = usacreport.FrmSacReport.f1jdll.removeARecord(where.getBytes());
      PreProcessor.WriteOptLogFile("报表数据库管理：删除报表[" + str +"]条件是[" + where +"]");
    }

    //structTree();
    //Freshlist();
    ((usacreport.FrmSacReport)getParent()).structTree();
    return ;
  }
    public String[] findFileByEndStr(String fileDir, String str) {
        ArrayList<String> list = new ArrayList<String>();
        String[] fileArray =null;
        if (null == fileDir) {
            throw new RuntimeException("要搜索的目录名不能为null");
        }
        File dir = new File(fileDir);
        if (!dir.exists()) {
            throw new RuntimeException(fileDir + "is not existed!");
        }
        if (null == str) {
            throw new RuntimeException("要匹配的末尾不能为null");
        }
        File[] files = dir.listFiles();
        for (File file : files) {

            if (file.isFile() && file.toString().endsWith(str)) {
                System.out.println("文件名:" + file.getName() + "\t\t绝对路径:"
                        + file.toString());
                list.add(file.getName());

            }
            if (file.isDirectory()) {
                findFileByEndStr(file.toString(), str);

            }

        }
        int size = list.size();
        fileArray = (String[]) list.toArray(new String[size]);
        return fileArray;

    }

  void Freshlist(){
    vecfile.removeAllElements();
    vectorin.removeAllElements();
    usacreport.dlgSelect.vectorprint.removeAllElements();

    String strDir = "";
    String filesp = System.getProperty("file.separator");
    strDir = usacreport.FrmSacReport.s_sacenv + filesp + "reportfile";
//    System.out.println(strDir);
    String str[] = findFileByEndStr(strDir, ".vts");
    int i = 0,j = 0;

    if(str.length > 0)
    {
      for(i = 0;i<str.length;i++){
        vecfile.addElement(str[i]);
      }
    }

    Vector vectortemp = new Vector();
    fieldname = "RptName,RptType,timePrintFlag,webID,printtime,papertype,direction,subid,temp1,temp2";
    int []icount = new int[1];
    int returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
                      order.getBytes(),icount,true);

    if(returnv > 0){
      byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);

      for(j = 0;j < returnv;j+=10)
      {
        String str1 = new String(buffer[j]);
        String str2 = new String(buffer[j+1]);
        String str3 = new String(buffer[j+2]);
        String str4 = new String(buffer[j+3]);
        String str5 = new String(buffer[j+4]);
        String str6 = new String(buffer[j+5]);
        String str7 = new String(buffer[j+6]);
        String str8 = new String(buffer[j+7]);
        String str9 = new String(buffer[j+8]);
        String str10 = new String(buffer[j+9]);
        str1 = str1.trim();
        //vectorin的内容(rptname,rpttype,timeprintflag,webid)
        vectorin.addElement(str1);
        str2 = str2.trim();
        vectorin.addElement(str2);
        str3 = str3.trim();
        vectorin.addElement(str3);
        str4 = str4.trim();
        vectorin.addElement(str4);
        str5 = str5.trim();
        vectorin.addElement(str5);
        str6 = str6.trim();
        vectorin.addElement(str6);
        str7 = str7.trim();
        vectorin.addElement(str7);
        str8 = str8.trim();
        vectorin.addElement(str8);
        str9 = str9.trim();
        vectorin.addElement(str9);
        str10 = str10.trim();
        vectorin.addElement(str10);
        if(str3.compareTo("1")==0){
          usacreport.dlgSelect.vectorprint.addElement(str1);
          usacreport.dlgSelect.vectorprint.addElement(str5);
          usacreport.dlgSelect.vectorprint.addElement(str6);
          usacreport.dlgSelect.vectorprint.addElement(str7);
          usacreport.dlgSelect.vectorprint.addElement("0");//判断在这一分钟内是否打印过
        }
      }
      freshtreec();
//      freshtreenode();
      Freshtype();
      int i_num = 0;
      i_num = vecfile.size();
      try{
        for(i=0;i<i_num;i++){
//          System.out.println(vecfile.get(i));
          for(j=0;j<vectorin.size();j+=10){
            if(i==i_num) break;
            if(vecfile.get(i).equals(vectorin.get(j) + ".vts")){
              vecfile.remove(i);
              i_num --;
              j=-10;
            }
          }
        }
        jListAll.setListData(vecfile);
      }
      catch(java.lang.IllegalArgumentException jCom){
      }
    }
    else{
      jListAll.setListData(vecfile);
    }
  }

  void jComtype_actionPerformed(ActionEvent e) {
    Freshtype();
  }

  void Freshtype(){
    Vector vectortemp = new Vector();
    int i = 0;
    jComday.setEnabled(false);
    jComhour.setEnabled(false);
    jCommin.setEnabled(false);
    jComptype.setEnabled(false);
    jComdir.setEnabled(false);
    try
    {
      switch(jComtype.getSelectedIndex())
      {
        case 0:
           {
           if (jComtypet.getItemCount() > 0) {
              for (int j = 0; j < jComtypet.getItemCount(); j++) {
                 if (jComtype.getSelectedItem().toString().equals(jComtypet.getItemAt(j).toString())) {
                    jComtypet.setSelectedItem(jComtype.getSelectedItem().
                                              toString());

                 }
              }
             }
             jComday.setEnabled(false);
             jComhour.setEnabled(true);
             jCommin.setEnabled(true);
             jComptype.setEnabled(true);
             jComdir.setEnabled(true);
             for(i=0;i<vectorin.size();i+=10){
               if(vectorin.get(i+1).equals("日报表") && vectorin.get(i+2).equals("1") && tree_subid.compareTo(vectorin.get(i+7).toString())==0){
                 vectortemp.addElement(vectorin.get(i));
               }
             }
             jListtype.setListData(vectortemp);
             break;
          }
        case 1:
        {
           if (jComtypet.getItemCount() > 0) {
              for (int j = 0; j < jComtypet.getItemCount(); j++) {
                 if (jComtype.getSelectedItem().toString().equals(jComtypet.
                     getItemAt(j).toString())) {
                    jComtypet.setSelectedItem(jComtype.getSelectedItem().
                                              toString());

                 }
              }
           }
           jComday.setEnabled(true);
           jComhour.setEnabled(true);
           jCommin.setEnabled(true);
           jComptype.setEnabled(true);
           jComdir.setEnabled(true);
           for(i=0;i<vectorin.size();i+=10){
             if(vectorin.get(i+1).equals("月报表") && vectorin.get(i+2).equals("1") && tree_subid.compareTo(vectorin.get(i+7).toString())==0){
               vectortemp.addElement(vectorin.get(i));
             }
           }
           jListtype.setListData(vectortemp);
           break;
        }
        case 2:
           {
              if (jComtypet.getItemCount() > 0) {
                 for (int j = 0; j < jComtypet.getItemCount(); j++) {
                    if (jComtype.getSelectedItem().toString().equals(jComtypet.
                        getItemAt(j).toString())) {
                       jComtypet.setSelectedItem(jComtype.getSelectedItem().
                                                 toString());

                    }
                 }
              }
             for(i=0;i<vectorin.size();i+=10){
               if(vectorin.get(i+1).equals("日报表") && vectorin.get(i+2).equals("0") && tree_subid.compareTo(vectorin.get(i+7).toString())==0){
                 vectortemp.addElement(vectorin.get(i));
               }
             }
             jListtype.setListData(vectortemp);
             break;
           }
        case 3:
          {
             if (jComtypet.getItemCount() > 0) {
                for (int j = 0; j < jComtypet.getItemCount(); j++) {
                   if (jComtype.getSelectedItem().toString().equals(jComtypet.
                       getItemAt(j).toString())) {
                      jComtypet.setSelectedItem(jComtype.getSelectedItem().
                                                toString());

                   }
                }
             }

            for(i=0;i<vectorin.size();i+=10){
              if(vectorin.get(i+1).equals("月报表") && tree_subid.compareTo(vectorin.get(i+7).toString())==0){
                vectortemp.addElement(vectorin.get(i));
              }
            }
            jListtype.setListData(vectortemp);
            break;
          }
        case 4:
          {
             if (jComtypet.getItemCount() > 0) {
                for (int j = 0; j < jComtypet.getItemCount(); j++) {
                   if (jComtype.getSelectedItem().toString().equals(jComtypet.
                       getItemAt(j).toString())) {
                      jComtypet.setSelectedItem(jComtype.getSelectedItem().
                                                toString());

                   }
                }
             }
            for(i=0;i<vectorin.size();i+=10){
              if(vectorin.get(i+1).equals("年报表") && tree_subid.compareTo(vectorin.get(i+7).toString())==0){
                vectortemp.addElement(vectorin.get(i));
              }
            }
            jListtype.setListData(vectortemp);
            break;
          }
          case 5: {
             if (jComtypet.getItemCount() > 0) {
                for (int j = 0; j < jComtypet.getItemCount(); j++) {
                   if (jComtype.getSelectedItem().toString().equals(jComtypet.
                       getItemAt(j).toString())) {
                      jComtypet.setSelectedItem(jComtype.getSelectedItem().
                                                toString());

                   }
                }
             }
             for (i = 0; i < vectorin.size(); i += 10) {
                if (vectorin.get(i + 1).equals("时报表") &&
                    tree_subid.compareTo(vectorin.get(i + 7).toString()) == 0) {
                   vectortemp.addElement(vectorin.get(i));
                }
             }
             jListtype.setListData(vectortemp);
             break;
          }
          case 6: {
             if (jComtypet.getItemCount() > 0) {
                for (int j = 0; j < jComtypet.getItemCount(); j++) {
                   if (jComtype.getSelectedItem().toString().equals(jComtypet.
                       getItemAt(j).toString())) {
                      jComtypet.setSelectedItem(jComtype.getSelectedItem().
                                                toString());

                   }
                }
             }
             for (i = 0; i < vectorin.size(); i += 10) {
                if (vectorin.get(i + 1).equals("周报表") &&
                    tree_subid.compareTo(vectorin.get(i + 7).toString()) == 0) {
                   vectortemp.addElement(vectorin.get(i));
                }
             }
             jListtype.setListData(vectortemp);
             break;
          }
          case 7: {
             if (jComtypet.getItemCount() > 0) {
                for (int j = 0; j < jComtypet.getItemCount(); j++) {
                   if (jComtype.getSelectedItem().toString().equals(jComtypet.
                       getItemAt(j).toString())) {
                      jComtypet.setSelectedItem(jComtype.getSelectedItem().
                                                toString());

                   }
                }
             }
             for (i = 0; i < vectorin.size(); i += 10) {
                if (vectorin.get(i + 1).equals("季报表") &&
                    tree_subid.compareTo(vectorin.get(i + 7).toString()) == 0) {
                   vectortemp.addElement(vectorin.get(i));
                }
             }
             jListtype.setListData(vectortemp);
             break;
          }

      }
    }
    catch(java.lang.IllegalArgumentException ejcom){
    }
  }
  //-------hongxiang
  void Freshtype1(){
      Vector vectortemp = new Vector();
      int i = 0;
      jComday.setEnabled(false);
      jComhour.setEnabled(false);
      jCommin.setEnabled(false);
      jComptype.setEnabled(false);
      jComdir.setEnabled(false);
      try
      {
        switch(jComtype.getSelectedIndex())
        {
          case 0:
             {
               jComday.setEnabled(false);
               jComhour.setEnabled(true);
               jCommin.setEnabled(true);
               jComptype.setEnabled(true);
               jComdir.setEnabled(true);
               for(i=0;i<vectorin.size();i+=10){
                 if(vectorin.get(i+1).equals("日报表") && vectorin.get(i+2).equals("1") && tree_subid.compareTo(vectorin.get(i+7).toString())==0){
                   vectortemp.addElement(vectorin.get(i));
                 }
               }
               jListtype.setListData(vectortemp);
               break;
            }
          case 1:
          {
             jComday.setEnabled(true);
             jComhour.setEnabled(true);
             jCommin.setEnabled(true);
             jComptype.setEnabled(true);
             jComdir.setEnabled(true);
             for(i=0;i<vectorin.size();i+=10){
               if(vectorin.get(i+1).equals("月报表") && vectorin.get(i+2).equals("1") && tree_subid.compareTo(vectorin.get(i+7).toString())==0){
                 vectortemp.addElement(vectorin.get(i));
               }
             }
             jListtype.setListData(vectortemp);
             break;
          }
          case 2:
             {
               for(i=0;i<vectorin.size();i+=10){
                 if(vectorin.get(i+1).equals("日报表") && vectorin.get(i+2).equals("0") && tree_subid.compareTo(vectorin.get(i+7).toString())==0){
                   vectortemp.addElement(vectorin.get(i));
                 }
               }
               jListtype.setListData(vectortemp);
               break;
             }
          case 3:
            {

              for(i=0;i<vectorin.size();i+=10){
                if(vectorin.get(i+1).equals("月报表") && tree_subid.compareTo(vectorin.get(i+7).toString())==0){
                  vectortemp.addElement(vectorin.get(i));
                }
              }
              jListtype.setListData(vectortemp);
              break;
            }
          case 4:
            {
              for(i=0;i<vectorin.size();i+=10){
                if(vectorin.get(i+1).equals("年报表") && tree_subid.compareTo(vectorin.get(i+7).toString())==0){
                  vectortemp.addElement(vectorin.get(i));
                }
              }
              jListtype.setListData(vectortemp);
              break;
            }
            case 5:
            {
               for (i = 0; i < vectorin.size(); i += 10) {
                  if (vectorin.get(i + 1).equals("时报表") &&
                      tree_subid.compareTo(vectorin.get(i + 7).toString()) == 0) {
                     vectortemp.addElement(vectorin.get(i));
                  }
               }
               jListtype.setListData(vectortemp);
               break;
            }
            case 6:
            {
               for (i = 0; i < vectorin.size(); i += 10) {
                  if (vectorin.get(i + 1).equals("周报表") &&
                      tree_subid.compareTo(vectorin.get(i + 7).toString()) == 0) {
                     vectortemp.addElement(vectorin.get(i));
                  }
               }
               jListtype.setListData(vectortemp);
               break;
            }
            case 7:
            {
               for (i = 0; i < vectorin.size(); i += 10) {
                  if (vectorin.get(i + 1).equals("季报表") &&
                      tree_subid.compareTo(vectorin.get(i + 7).toString()) == 0) {
                     vectortemp.addElement(vectorin.get(i));
                  }
               }
               jListtype.setListData(vectortemp);
               break;
            }

        }
      }
      catch(java.lang.IllegalArgumentException ejcom){
      }
  }
  //------hongxiang
  void jListtype_mouseClicked(MouseEvent e) {
    OnButDel();
  }

  private void createNodes(DefaultMutableTreeNode top) {
        DefaultMutableTreeNode category = null;
        DefaultMutableTreeNode book = null;

        int i=0,j=0;
        String dbname = "modeldb";
        String tablename = "webRptName";
        String fieldname = "RptName";
        String where = "",order = "";
        int []icount = new int[1];
        int returnv = 0;
        for(i=0;i<usacreport.FrmSacReport.dlgtreem1.vec_treenodeid.size();i++){
          category = new DefaultMutableTreeNode((String)usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.get(i));
          top.add(category);

          where = "subid = " + (String)usacreport.FrmSacReport.dlgtreem1.vec_treenodeid.get(i);
          returnv = usacreport.FrmSacReport.f1jdll.readRecorders(dbname.getBytes(),tablename.getBytes(),fieldname.getBytes(),where.getBytes(),
              order.getBytes(),icount,true);

          if(returnv > 0){
            byte [][] buffer = new byte[0][0];
            buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
            int m_ret = usacreport.FrmSacReport.f1jdll.readdata(buffer);
            for(j=0;j<m_ret;j++){
              String strtemp = new String(buffer[j]);
              book = new DefaultMutableTreeNode(strtemp.trim());
              category.add(book);
            }
          }
        }
    }

    void structTree(){
      DefaultMutableTreeNode top =
            new DefaultMutableTreeNode("ROOT");
        createNodes(top);

        //Create a tree that allows one selection at a time.
        usacreport.FrmSacReport.jTreetab = new JTree(top);
        usacreport.FrmSacReport.jTreetab.setRootVisible(false);
        usacreport.FrmSacReport.jTreetab.setFont(new java.awt.Font("Dialog", 0, 20));
        usacreport.FrmSacReport.jTreetab.setRowHeight(26);
        usacreport.FrmSacReport.jTreetab.getSelectionModel().setSelectionMode
                (TreeSelectionModel.SINGLE_TREE_SELECTION);
        usacreport.FrmSacReport.jScrollTree.getViewport().add(usacreport.FrmSacReport.jTreetab, null);
        //Listen for when the selection changes.

        MouseListener ml = new MouseAdapter() {
          public void mousePressed(MouseEvent e) {
            int selRow = usacreport.FrmSacReport.jTreetab.getRowForLocation(e.getX(), e.getY());
            javax.swing.tree.TreePath selPath = usacreport.FrmSacReport.jTreetab.getPathForLocation(e.getX(), e.getY());
            if(selRow != -1) {

              if(e.getClickCount() == 1) {
//                mySingleClick(selRow, selPath);
              }
              else if(e.getClickCount() == 2) {
                javax.swing.tree.TreePath tPath = usacreport.FrmSacReport.jTreetab.getSelectionPath();
                String nodename = String.valueOf(tPath.getLastPathComponent());
                for(int i=0;i<usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.size();i++){
                  if(nodename.compareTo(usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.get(i).toString())==0){
                    return;
                  }
                }
                Dimension dlgSize = usacreport.FrmSacReport.dlgsel.getPreferredSize();
                Dimension frmSize = getSize();
                Point loc = getLocation();
                usacreport.FrmSacReport.dlgsel.setLocation((frmSize.width - dlgSize.width) / 2 + loc.x, (frmSize.height - dlgSize.height) / 2 + loc.y);
                usacreport.FrmSacReport.dlgsel.setModal(true);
                usacreport.FrmSacReport.dlgsel.pack();
                usacreport.FrmSacReport.dlgsel.g_selfilename = nodename;
                usacreport.FrmSacReport.dlgsel.show();
              }
            }
          }
        };
        usacreport.FrmSacReport.jTreetab.addMouseListener(ml);
  }
  void jComtypet_actionPerformed(ActionEvent e) {
        Freshtypet();
        freshtreenode();
  }

  void Freshtypet(){
     if (jComtypet.getItemCount() > 0) {
        if (jComtype.getItemCount() > 0) {

           for (int j = 0; j < jComtype.getItemCount(); j++) {
              if (jComtypet.getSelectedItem().toString().equals(jComtype.
                  getItemAt(j).toString())) {
                 jComtype.setSelectedItem(jComtypet.getSelectedItem().
                                          toString());

              }
           }
        }
     }
  }
  void freshtreenode (){
    String str_tree = "";
    for(int i=0;i<usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.size();i++){
      str_tree = (String)usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.get(i);
      if(jComtypet.getSelectedIndex() < 0) return;
      if(str_tree.compareTo(jComtypet.getSelectedItem().toString())==0){
        tree_subid = (String)usacreport.FrmSacReport.dlgtreem1.vec_treenodeid.get(i);
        break;
      }
    }
    Freshtype1();
  }
  void freshtreec(){
    jComtypet.removeAllItems();
    for(int i = 0;i<usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.size();i++){
      jComtypet.addItem(usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.get(i));
    }
  }
  void freshalllist(){
    jListAll.setListData(vecfile);
  }

  void jListAll_mouseClicked(MouseEvent e) {

  }
  void OnButAdd(){
    String Stypet=jComtypet.getSelectedItem().toString();
    int pos = jListAll.getSelectedIndex();
    if(pos < 0 ){
      return;
    }
    String str = (String)jListAll.getSelectedValue();
    str = str.substring(0,str.lastIndexOf("."));
    String type = "",flag = "",webid = "0",printtime = "0",papertype = "0",m_dir="0",subid="",temp1="",temp2="",groupname="";

    fieldname = "RptName";
    where = "";
    int i=0,j=0;
    String tempstr = "";
    for(i=0;i<vectorin.size();i+=10){
      if(Integer.parseInt(webid) < Integer.parseInt(vectorin.get(i+3).toString())){
        webid = (String)vectorin.get(i+3);
      }
    }
    if(vectorin.size() == 0){
      webid = Integer.toString(0);
    }else{
      webid = Integer.toString(Integer.parseInt(webid) + 1);
    }

    try
    {
      switch(jComtype.getSelectedIndex())
      {
        case 0:
          type = "日报表";
          flag = "1";
          printtime = Integer.toString(jComhour.getSelectedIndex()*60 + jCommin.getSelectedIndex());
          papertype = Integer.toString(jComptype.getSelectedIndex());
          m_dir = Integer.toString(jComdir.getSelectedIndex());
          break;
       case 1:
          type = "月报表";
          flag = "1";
          printtime = Integer.toString((jComday.getSelectedIndex()+1)*3600+jComhour.getSelectedIndex() * 60 +
                                       jCommin.getSelectedIndex());
          papertype = Integer.toString(jComptype.getSelectedIndex());
          m_dir = Integer.toString(jComdir.getSelectedIndex());
          break;
        case 2:
          type = "日报表";
          flag = "0";
          break;
       case 3:
          type = "月报表";
          flag = "0";
          break;
       case 4:
          type = "年报表";
          flag = "0";
          break;
       case 5:
          type = "时报表";
          flag = "0";
          break;
       case 6:
          type = "周报表";
          flag = "0";
          break;
       case 7:
          type = "季报表";
          flag = "0";
          break;



      }

    }
    catch(java.lang.IllegalArgumentException ejcom){
    }
    try{
      if (jComGroup.getSelectedIndex() > 0)
         groupname = jComGroup.getSelectedItem().toString();
      else if (jComGroup.getSelectedItem().toString() != null)
         groupname = jComGroup.getSelectedItem().toString();
      else
         groupname="";
    }
    catch(Exception e){
       if (jComGroup.getSelectedItem().toString() != null)
         groupname = jComGroup.getSelectedItem().toString();
      else
         groupname="";

    }
//    Freshtype();
    int []icount = new int[1];
    String s_find = "";
    for(i=0;i<usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.size();i++){
      s_find = (String)usacreport.FrmSacReport.dlgtreem1.vec_treenodedec.get(i);
      if(s_find.compareTo(jComtypet.getSelectedItem().toString())==0){
        subid = (String)usacreport.FrmSacReport.dlgtreem1.vec_treenodeid.get(i);
        break;
      }
    }
    vectorin.addElement(str);
    vectorin.addElement(type);
    vectorin.addElement(flag);
    vectorin.addElement(webid);
    vectorin.addElement(printtime);
    vectorin.addElement(papertype);
    vectorin.addElement(m_dir);
    vectorin.addElement(subid);
    vectorin.addElement(temp1);
    vectorin.addElement(temp2);
    int returnv = usacreport.FrmSacReport.f1jdll.openTableInfo(dbname.getBytes(),tablename.getBytes());

    if(returnv > 0){
      byte [][] buffer = new byte[0][0];
      buffer = new byte[returnv][usacreport.ConstantDef.MAX_COLUMN_LEN];
      buffer[0] = str.getBytes();
      buffer[1] = type.getBytes();
      buffer[2] = flag.getBytes();
      buffer[3] = webid.getBytes();
      buffer[4] = printtime.getBytes();
      buffer[5] = papertype.getBytes();
      buffer[6] = m_dir.getBytes();
      buffer[7] = subid.getBytes();
      buffer[8] = temp1.getBytes();
      buffer[9] = temp2.getBytes();
      buffer[10]=groupname.getBytes();             //因分组而添加
      int m_ret = usacreport.FrmSacReport.f1jdll.insertARecord(buffer);
      //System.out.println("Insert 报表名!");
      usacreport.FrmSacReport.f1jdll.closeTableInfo();

      if(m_ret > 0){
        vecfile.remove(pos);
        jListAll.setListData(vecfile);
        FillGroupname();  //添加成功后重新初始化下分组名列表
        //write operat log
        PreProcessor.WriteOptLogFile("报表数据库管理：添加报表[" + str +"]到[" + type +"]中");

      }
    }


    Freshtype();
    //可以解决加入定时报表后打印数组不更新的问题，但删除报表不能使用该函数。
    //目前加入新定时报表后可以直接不用退出就生效。
    Freshlist();
    ((usacreport.FrmSacReport)getParent()).structTree();
    //structTree();
    jComtypet.setSelectedItem(Stypet);
  }

  void OnButDel(){
    if(jComtype.getSelectedIndex() == 0){//定时打印报表要处理显示时间的问题
      int pos = jListtype.getSelectedIndex();
      if(pos < 0) return;

      String str = (String)jListtype.getSelectedValue();
      for(pos = 0;pos<vectorin.size();pos+=10){
        String str1 = (String)vectorin.get(pos);
        if(str1.compareTo(str) == 0){
          int hour = Integer.parseInt((String)vectorin.get(pos+4));
          jComhour.setSelectedIndex(hour/60);
          jCommin.setSelectedIndex(hour%60);
          int size = Integer.parseInt((String)vectorin.get(pos+5));
          jComptype.setSelectedIndex(size);
          int m_dir = Integer.parseInt((String)vectorin.get(pos+6));
          jComdir.setSelectedIndex(m_dir);
        }
      }
    }
  }

  private void FillGroupname(){
     int nColCnt;
     Vector vecName=new Vector();
     nColCnt=DBEx.SqlReadRecorders(dbname,"Select distinct groupname from webrptname where groupname<>' '",vecName);
     if(nColCnt>0){
        jComGroup.removeAllItems();
        for(int i=0;i<vecName.size();i+=nColCnt){
           jComGroup.addItem(vecName.get(i));
        }
     }
  }
}
