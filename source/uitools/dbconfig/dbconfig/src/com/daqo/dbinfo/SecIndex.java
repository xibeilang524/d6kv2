package com.daqo.dbinfo;

import java.util.Vector;
import javax.swing.*;
import java.awt.event.*;
import java.awt.*;

import com.daqo.dbinfo.table.*;
import javax.swing.text.Document;
import javax.swing.event.DocumentEvent;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//二次分类检索
public class SecIndex {

  String dbname;
  String datsrcname; //初始化信息源名
  String datsrclistname; //列表域名
  String searchedfield; //信息域名
  Dlllink pdll = Dlllink.getInstance();
  String searchedfieldname; //被检索域名
  String searchfieldname; //检索域名
  String showfieldname; //显示域名
  String orderfd; //排序域名
  String conditionflag; //条件域值
  String conditionfield; //选择域名
  String valuefrom; //选择域初值
  String valueto; //选择域终值
  SecondIndexEditor siEditor = null;

  String fieldname = "";
  String where = "";
  String select = ""; //add by hongxiang 关联设备代码时快速定位所选的站所线系
  ReadPDBTable readpdbtable = new ReadPDBTable();

  public SecIndex(String DBName,String selectSub) {
    dbname = DBName;
    select = selectSub;
  }

//初始化信息源名,为空return true.
  public boolean isDatSrcNameEmpty() {
    if (datsrcname.equalsIgnoreCase("")) {
      return true;
    }
    else {
      return false;
    }
  }

//列表域名,为空,return true.
  public boolean isListNameEmpty() {
    if (datsrclistname.equalsIgnoreCase("")) {
      return true;
    }
    else {
      return false;
    }
  }

  //信息域名;为空,return true.
  public boolean isSearchedFieldEmpty() {
    if (searchedfield.equalsIgnoreCase("")) {
      return true;
    }
    else {
      return false;
    }
  }

  //检查基本的设置。满足return true,否则return false
  public boolean baseCheckBeforeRun() {
    if (isDatSrcNameEmpty()) {
      return false;
    }
    if (isListNameEmpty() && isSearchedFieldEmpty()) {
      return false;
    }
    return true;
  }

  //add by hongxiang 由终端描述获取站所线系代码
  public String getSubCode(String desc){

    String wh = "描述='" + desc + "'";
    byte[][] buf = pdll.queryAnyTable(ConstantDef.MODELDB, "终端参数表",
            "站所线系代码", wh, "", new int[1], false);
    return pdll.substr(buf[0]);

  }
  //add by hongxiang 由站所线系代码获取站所线系描述
  public String getSubDesc(String subCode){

    String wh = "代码='" + subCode + "'";
    byte[][] buf = pdll.queryAnyTable(ConstantDef.MODELDB, "站所线系参数表",
            "描述", wh, "", new int[1], false);
    return pdll.substr(buf[0]);

  }


  public boolean doFirstIndex() {
    if (baseCheckBeforeRun()) { //基本设置对了，才进行
      String whnext = ConstantDef.TABLENAME + "='" + datsrcname + "' and " +
          ConstantDef.SORTINDEX + ">0";

      int recs = readpdbtable.getPDBPropwhere(whnext) ;
      if (recs > 0) { //初始化信息源名有索引
        searchedfieldname = readpdbtable.getAField(ConstantDef.SEARCHEDFIELDID); //被检索域名
        searchfieldname = readpdbtable.getAField(ConstantDef.SEARCHFIELDID); //检索域名
        showfieldname = readpdbtable.getAField(ConstantDef.SHOWFIELDID); //显示域名
        orderfd = readpdbtable.getAField(ConstantDef.SORTFIELDID); //排序域名

        if (showfieldname.equals("") && !searchedfieldname.equals("")) {
          fieldname = searchedfieldname + "," + searchedfieldname;
        }
        else if (!showfieldname.equals("") && searchedfieldname.equals("")) {
          fieldname = showfieldname + "," + showfieldname;
        }
        else if (!showfieldname.equals("") && !searchedfieldname.equals("")) {
          fieldname = searchedfieldname + "," + showfieldname; //先检索域名，后显示域名
          if (searchedfieldname.equalsIgnoreCase(showfieldname)) {
            fieldname = searchedfieldname;
          }
        }
        return true;
      }
    }
    return false;
  }

//
  public void doSecondIndex() {

    final Vector vContent = new Vector(0);
    final Vector vList = new Vector(0);
    if (!fieldname.equalsIgnoreCase("")) {
      pdll.queryAnyTable(dbname,
                         readpdbtable.getAField(ConstantDef.SEARCHEDTABLEID),
                         fieldname, where, "描述", vContent, true);
//                         fieldname, where, orderfd, vContent, true);
      if (searchedfieldname.equalsIgnoreCase(showfieldname)) {
        vContent.add(1, (Vector) (vContent.elementAt(0)));
      }
    }

    String[] typefieldname = new String[2];
    typefieldname[0] = searchfieldname; //检索域名
    typefieldname[1] = conditionfield; //选择域名
    final int[] fieldtype = pdll.getFieldsType(dbname.trim(), datsrcname.trim(),
                                               typefieldname); //检索域名,选择域名的数据类型

    final JButton button = new JButton("") {
      public void setText(String s) {
        //Button never shows text -- only color.
      }
    };
    button.doClick(2);
    button.setBackground(Color.white);
    button.setBorderPainted(false);
    button.setMargin(new Insets(0, 0, 0, 0));

    //Now create an editor to encapsulate the button, and
    //set it up as the editor for all Color cells.
    siEditor = new SecondIndexEditor(button);

    //Set up the dialog that the button brings up.
    ActionListener okListener = new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        //siEditor.currentString = "5678";
      }
    };

    final ReadDB readdb = new ReadDB(dbname.trim(), datsrcname.trim());

    final SecDialog dialog = new SecDialog(ApplicationDB.frameDB, "输入对话框", true);
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();

    ImageIcon imagefind = new ImageIcon(com.daqo.dbinfo.FrameDB.class.getResource(
          "icon/menuundel.gif"));
    JButton jButtonFind =new JButton();
    JToolBar jToolBarFind = new JToolBar();
    JPanel  jPanleleft = new JPanel();
    final JTextField FindTxt = new JTextField();
    JPanel jPanel1 = new JPanel();

    JButton jButtonCancle = new JButton();
    JButton jButtonOK = new JButton();
    GridLayout gridLayout1 = new GridLayout();
    BorderLayout borderLayout1 = new BorderLayout();
    final JComboBox comboBox = new JComboBox( (Vector) vContent.elementAt(1));
    comboBox.setSelectedIndex(-1);

    //add by hongxiang 快速定位所选站所线系
    ComboBoxModel comboBoxModel = comboBox.getModel();
    for(int i = 0; i < comboBoxModel.getSize(); i++){
      String elem = comboBoxModel.getElementAt(i).toString();
      //由终端描述获取站所线系描述
      String subDesc = getSubDesc(getSubCode(select));
      //定位站所线系
      if(subDesc.equals(elem)){
        comboBox.setSelectedIndex(i);
        //找到了就跳出循环了，尼玛找累死了。
        break;
      }
    }
    //add end

    JScrollPane scrollPane = new JScrollPane();
    final DefaultListModel listModel = new DefaultListModel();
    final JList jlist = new JList(listModel);

    final DefaultListModel listModel1 = new DefaultListModel();
    final JList jlist1 = new JList(listModel1);
    jlist.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
    //add by hongxiang 根据下拉菜单的初始状态，设置List
    if(comboBox.getSelectedIndex() != -1){
      FindTxt.setText(null);
      int selectindex = comboBox.getSelectedIndex();
      Vector v0 = (Vector) vContent.elementAt(0);
      Vector v1 = (Vector) vContent.elementAt(1);
      int vsize = v0.size();
      if (selectindex >= 0 && selectindex < vsize) {
        String item0 = v0.elementAt(selectindex).toString().trim(); //检索域名的值
        if (item0.equals("")) {
          return;
        }
        String we = "";

        switch (fieldtype[0]) { //初始化信息源名的检索域名的类型
          case ConstantDef.STRINGTYPE: {
            we = searchfieldname + "='" + item0 + "'";
            break;
          }
          case ConstantDef.FLOATTYPE: {
            we = searchfieldname + "=" + Float.parseFloat(item0);
            break;
          }
          case ConstantDef.DATATYPE: {
            we = searchfieldname + "=" + Integer.parseInt(item0);
          }
        }
        if ( (!conditionflag.equalsIgnoreCase("0")) &&
                (!conditionfield.equalsIgnoreCase("")) &&
                ( (!valuefrom.equalsIgnoreCase("")) ||
                        !valueto.equalsIgnoreCase(""))) { //有条件域值可选择
          we = we + "  and  ";
          switch (fieldtype[1]) {
            case ConstantDef.DATATYPE: {
              if (valuefrom.equalsIgnoreCase("") &&
                      !valueto.equalsIgnoreCase("")) {
                we = conditionfield + "<=" + Integer.parseInt(valueto);
              }
              else if (!valuefrom.equalsIgnoreCase("") &&
                      valueto.equalsIgnoreCase("")) {
                we = conditionfield + ">=" + Integer.parseInt(valuefrom);
              }
              else {
                we = conditionfield + ">=" + Integer.parseInt(valuefrom) +
                        " and  " + conditionfield + "<=" +
                        Integer.parseInt(valueto);
              }
              break;
            }
            case ConstantDef.FLOATTYPE: {
              if (valuefrom.equalsIgnoreCase("") &&
                      !valueto.equalsIgnoreCase("")) {
                we = conditionfield + "<=" + Float.parseFloat(valueto);
              }
              else if (!valuefrom.equalsIgnoreCase("") &&
                      valueto.equalsIgnoreCase("")) {
                we = conditionfield + ">=" + Float.parseFloat(valuefrom);
              }
              else {
                we = conditionfield + ">=" + Float.parseFloat(valuefrom) +
                        " and " + conditionfield + "<=" +
                        Float.parseFloat(valueto);
              }
              break;
            }
            case ConstantDef.STRINGTYPE: {
              we = conditionfield + ">= '" + valuefrom + "' and " +
                      conditionfield + "<= '" + valueto + "'";
              break;
            } //case
          } //switch
        } //if

        String fd = ""; //首选 信息域名
        if (!searchedfield.equalsIgnoreCase("") &&
                !datsrclistname.equalsIgnoreCase("")) { //信息域名 列表域名都不为空
          if (searchedfield.equalsIgnoreCase(datsrclistname)) {
            fd = searchedfield;
          }
          else {
            fd = searchedfield + "," + datsrclistname; //信息域名 + 列表域名
          }
        }
        else if (searchedfield.equalsIgnoreCase("")) {
          fd = datsrclistname;
        }
        else {
          fd = searchedfield;
        }
        readdb.queryTable(fd.trim(), we.trim(), datsrclistname);
        listModel.removeAllElements();
        jlist.removeAll() ;
        listModel1.removeAllElements();
        jlist1.removeAll() ;
        if (readdb.getFields()  == 1) { //只有1个域时
          for (int mm = 0; mm < readdb.getRecords() ; mm++) {
            listModel.addElement(readdb.getAFieldValue(mm,0) );
            listModel1.addElement(readdb.getAFieldValue(mm,0) );
          }
        }
        else {
          for (int mm = 0; mm < readdb.getRecords() ; mm++) {
            listModel.addElement(readdb.getAFieldValue(mm,1));
            listModel1.addElement(readdb.getAFieldValue(mm,1) );
          }
        }
      }
    }//add by hongxiang end

    dialog.getContentPane().setLayout(borderLayout1);
    jPanel1.setMinimumSize(new Dimension(35, 35));
    jPanel1.setPreferredSize(new Dimension(35, 35));
    jPanel1.setLayout(gridLayout1);
    jButtonCancle.setPreferredSize(new Dimension(35, 55));
    jButtonOK.setPreferredSize(new Dimension(35, 55));
    comboBox.setMaximumSize(new Dimension(32767, 25));
    comboBox.setMinimumSize(new Dimension(122, 25));
    comboBox.setPreferredSize(new Dimension(126, 25));


    dialog.getContentPane().add(jPanleleft, BorderLayout.CENTER);
    dialog.getContentPane().add(jPanel1, BorderLayout.SOUTH);

    jPanleleft.setLayout(new BorderLayout());
    jPanleleft.add(scrollPane, BorderLayout.CENTER);
    jPanleleft.add(jToolBarFind,BorderLayout.NORTH);

    scrollPane.getViewport().add(jlist, BorderLayout.CENTER);

    jToolBarFind.setPreferredSize(new Dimension(20, 30));
    jToolBarFind.setFloatable(false);
    jButtonFind.setBackground(new Color(238, 238, 238));
    jButtonFind.setIcon(imagefind);
    jToolBarFind.add(FindTxt);
    //jToolBarFind.add(jButtonFind);

    jPanel1.add(jButtonOK, null);
    jPanel1.add(jButtonCancle, null);
    jButtonOK.setText("确定");
    jButtonCancle.setText("取消");
    dialog.getContentPane().add(comboBox, BorderLayout.NORTH);
    dialog.setSize(screenSize.width / 6, screenSize.height / 2);


    jButtonOK.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        dialog.setCancelF( false);
        dialog.dispose();
      }
    });

    jButtonCancle.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(ActionEvent e) {
        dialog.setCancelF( true);
        dialog.dispose();
      }
    });

    jlist.addMouseListener(new java.awt.event.MouseListener() {
      public void mouseClicked(MouseEvent e) {
        if (e.getClickCount() == 2) {
          dialog.setCancelF( false);
          dialog.dispose();
        }
      }

      public void mouseEntered(MouseEvent e) {
      }

      public void mouseExited(MouseEvent e) {
      }

      public void mousePressed(MouseEvent e) {
      }

      public void mouseReleased(MouseEvent e) {
      }
    });

    jlist.addKeyListener(new java.awt.event.KeyListener() {
      public void keyPressed(KeyEvent e) {
        if (e.getKeyCode() == e.VK_ENTER) {
          dialog.setCancelF( false);
          dialog.dispose();
        }
      }

      public void keyReleased(KeyEvent e) {

      }

      public void keyTyped(KeyEvent e) {

      }
    });

    comboBox.addItemListener(new java.awt.event.ItemListener() { //comboBox选了以后，要填充list的内容
      public void itemStateChanged(ItemEvent e) {
        FindTxt.setText(null);
        int selectindex = comboBox.getSelectedIndex();
        Vector v0 = (Vector) vContent.elementAt(0);
        Vector v1 = (Vector) vContent.elementAt(1);
        int vsize = v0.size();
        if (selectindex >= 0 && selectindex < vsize) {
          String item0 = v0.elementAt(selectindex).toString().trim(); //检索域名的值
          if (item0.equals("")) {
            return;
          }
          String we = "";

          switch (fieldtype[0]) { //初始化信息源名的检索域名的类型
            case ConstantDef.STRINGTYPE: {
              we = searchfieldname + "='" + item0 + "'";
              break;
            }
            case ConstantDef.FLOATTYPE: {
              we = searchfieldname + "=" + Float.parseFloat(item0);
              break;
            }
            case ConstantDef.DATATYPE: {
              we = searchfieldname + "=" + Integer.parseInt(item0);
            }
          }
          if ( (!conditionflag.equalsIgnoreCase("0")) &&
              (!conditionfield.equalsIgnoreCase("")) &&
              ( (!valuefrom.equalsIgnoreCase("")) ||
               !valueto.equalsIgnoreCase(""))) { //有条件域值可选择
            we = we + "  and  ";
            switch (fieldtype[1]) {
              case ConstantDef.DATATYPE: {
                if (valuefrom.equalsIgnoreCase("") &&
                    !valueto.equalsIgnoreCase("")) {
                  we = conditionfield + "<=" + Integer.parseInt(valueto);
                }
                else if (!valuefrom.equalsIgnoreCase("") &&
                         valueto.equalsIgnoreCase("")) {
                  we = conditionfield + ">=" + Integer.parseInt(valuefrom);
                }
                else {
                  we = conditionfield + ">=" + Integer.parseInt(valuefrom) +
                      " and  " + conditionfield + "<=" +
                      Integer.parseInt(valueto);
                }
                break;
              }
              case ConstantDef.FLOATTYPE: {
                if (valuefrom.equalsIgnoreCase("") &&
                    !valueto.equalsIgnoreCase("")) {
                  we = conditionfield + "<=" + Float.parseFloat(valueto);
                }
                else if (!valuefrom.equalsIgnoreCase("") &&
                         valueto.equalsIgnoreCase("")) {
                  we = conditionfield + ">=" + Float.parseFloat(valuefrom);
                }
                else {
                  we = conditionfield + ">=" + Float.parseFloat(valuefrom) +
                      " and " + conditionfield + "<=" +
                      Float.parseFloat(valueto);
                }
                break;
              }
              case ConstantDef.STRINGTYPE: {
                we = conditionfield + ">= '" + valuefrom + "' and " +
                    conditionfield + "<= '" + valueto + "'";
                break;
              } //case
            } //switch
          } //if

          String fd = ""; //首选 信息域名
          if (!searchedfield.equalsIgnoreCase("") &&
              !datsrclistname.equalsIgnoreCase("")) { //信息域名 列表域名都不为空
            if (searchedfield.equalsIgnoreCase(datsrclistname)) {
              fd = searchedfield;
            }
            else {
              fd = searchedfield + "," + datsrclistname; //信息域名 + 列表域名
            }
          }
          else if (searchedfield.equalsIgnoreCase("")) {
            fd = datsrclistname;
          }
          else {
            fd = searchedfield;
          }
          readdb.queryTable(fd.trim(), we.trim(), datsrclistname);
          listModel.removeAllElements();
          jlist.removeAll() ;
          listModel1.removeAllElements();
          jlist1.removeAll() ;
          if (readdb.getFields()  == 1) { //只有1个域时
            for (int mm = 0; mm < readdb.getRecords() ; mm++) {
              listModel.addElement(readdb.getAFieldValue(mm,0) );
              listModel1.addElement(readdb.getAFieldValue(mm,0) );
            }
          }
          else {
            for (int mm = 0; mm < readdb.getRecords() ; mm++) {
                listModel.addElement(readdb.getAFieldValue(mm,1));
                listModel1.addElement(readdb.getAFieldValue(mm,1) );
            }
          }
        }
      }
    });

      //--------------hx
          Document dt = FindTxt.getDocument();
          dt.addDocumentListener(new javax.swing.event.DocumentListener(){
              /**
               * Gives notification that an attribute or set of attributes changed.
               *
               * @param e the document event
               */
              public void changedUpdate(DocumentEvent e) {
                  // TODO 自动生成方法存根
                  String str = FindTxt.getText();
//                  str = str.toLowerCase();
                  Vector vFind = new Vector(0);
                  if (str.length() == 0) {
                    listModel.removeAllElements();
                    jlist.removeAll();
                    if (readdb.getFields() == 1) { //只有1个域时
                      for (int mm = 0; mm < readdb.getRecords(); mm++) {
                        listModel.addElement(readdb.getAFieldValue(mm, 0));
                      }
                    }
                    else {
                      for (int mm = 0; mm < readdb.getRecords(); mm++) {
                        listModel.addElement(readdb.getAFieldValue(mm, 1));
                      }
                    }
                    return;
                  }
                  for (int i = 0; i < jlist1.getModel().getSize(); i++) {

//                    int index = jlist1.getModel().getElementAt(i).toString().toLowerCase().indexOf(str);
                    int index = jlist1.getModel().getElementAt(i).toString().indexOf(str);
                    if (index >= 0) {
                      //System.out.println(jlist1.getModel().getElementAt(i));
                      // jlist.ABORT
                      vFind.add(jlist1.getModel().getElementAt(i));
                    }
                  }
                  listModel.removeAllElements();
                  jlist.removeAll();
                  for (int i = 0; i < vFind.size(); i++) {
                    listModel.addElement(vFind.get(i));
                  }


              }
              /**
               * Gives notification that there was an insert into the document. The
               * range given by the DocumentEvent bounds the freshly inserted region.
               *
               * @param e the document event
               */
              public void insertUpdate(DocumentEvent e) {
                  // TODO 自动生成方法存根
                  String str = FindTxt.getText();
//                System.out.println(str + "呵呵");
//                  str = str.toLowerCase();
                  Vector vFind = new Vector(0);
                  if (str.length() == 0) {
                                     listModel.removeAllElements();
                                     jlist.removeAll();
                                     if (readdb.getFields() == 1) { //只有1个域时
                                       for (int mm = 0; mm < readdb.getRecords(); mm++) {
                                         listModel.addElement(readdb.getAFieldValue(mm, 0));
                                       }
                                     }
                                     else {
                                       for (int mm = 0; mm < readdb.getRecords(); mm++) {
                                         listModel.addElement(readdb.getAFieldValue(mm, 1));
                                       }
                                     }
                                     return;
                  }
                  for (int i = 0; i < jlist1.getModel().getSize(); i++) {

//                    int index = jlist1.getModel().getElementAt(i).toString().toLowerCase().indexOf(str);
                    int index = jlist1.getModel().getElementAt(i).toString().indexOf(str);
//                    System.out.println(jlist1.getModel().getElementAt(i).toString() + "呵呵");
                    if (index >= 0) {
                      System.out.println(jlist1.getModel().getElementAt(i));
                      // jlist.ABORT
                      vFind.add(jlist1.getModel().getElementAt(i));
                    }
                  }
                  listModel.removeAllElements();
                  jlist.removeAll();
                  for (int i = 0; i < vFind.size(); i++) {
                    listModel.addElement(vFind.get(i));
                  }

              }
              /**
               * Gives notification that a portion of the document has been
               * removed. The range is given in terms of what the view last
               * saw (that is, before updating sticky positions).
               *
               * @param e the document event
               */
              public void removeUpdate(DocumentEvent e) {
                  // TODO 自动生成方法存根
                  String str = FindTxt.getText();
//                  str=str.toLowerCase();
                  Vector vFind = new Vector(0);
                  if (str.length() == 0) {
                                     listModel.removeAllElements();
                                     jlist.removeAll();
                                     if (readdb.getFields() == 1) { //只有1个域时
                                       for (int mm = 0; mm < readdb.getRecords(); mm++) {
                                         listModel.addElement(readdb.getAFieldValue(mm, 0));
                                       }
                                     }
                                     else {
                                       for (int mm = 0; mm < readdb.getRecords(); mm++) {
                                         listModel.addElement(readdb.getAFieldValue(mm, 1));
                                       }
                                     }
                                     return;
                  }
                  for (int i = 0; i < jlist1.getModel().getSize(); i++) {

//                    int index = jlist1.getModel().getElementAt(i).toString().toLowerCase().indexOf(str);
                    int index = jlist1.getModel().getElementAt(i).toString().indexOf(str);
                    if (index >= 0) {
                      System.out.println(jlist1.getModel().getElementAt(i));
                      // jlist.ABORT
                      vFind.add(jlist1.getModel().getElementAt(i));
                    }
                  }
                  listModel.removeAllElements();
                  jlist.removeAll();
                  for (int i = 0; i < vFind.size(); i++) {
                    listModel.addElement(vFind.get(i));
                  }

                }

                });


    //-------------hx

    //Here's the code that brings up the dialog.
    button.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        //Without the following line, the dialog comes up
        //in the middle of the screen.
        dialog.setLocationRelativeTo(button);
        dialog.show();
        //在此改变内容
        if (readdb.getFields()  == 1) { //不同时有列表域名和信息域名
          if (jlist.getSelectedIndex() >= 0 && dialog.getCancelF()  == false) {
            siEditor.currentString = jlist.getSelectedValue();
          }
        }
        else {
          int jlistindex = jlist.getSelectedIndex();
          if (jlistindex >= 0 && dialog.getCancelF()  == false) {
            if (FindTxt.getText().length()==0){
               siEditor.currentString = readdb.getAFieldValue(jlistindex, 0);
            }
            else {
              for (int i = 0; i < readdb.getRecords(); i++) {
                if (readdb.getAFieldValue(i, 1) == jlist.getSelectedValue())
                  siEditor.currentString = readdb.getAFieldValue(i, 0);
              }
            }
          }
        }
      }
    });
  }
}
