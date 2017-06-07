package com.daqo.dbinfo;

import java.util.Vector;
import javax.swing.*;
import java.awt.event.*;

import com.daqo.dbinfo.table.*;
import java.awt.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class SecIndex2 {

  String dbname;
  String datsrcname1; //初始化信息源名
  String datsrclistname; //列表域名
  String searchedfield; //信息域名
  Dlllink pdll = Dlllink.getInstance();
  String searchedfieldname; //被检索域名
  String searchfieldname; //检索域名
  String showfieldname; //显示域名
  String orderfd; //排序域名

  Vector vdatsrcname = new Vector(0); //存放分解后的初始化信息源名

  SecondIndexEditor siEditor = null;

  String fieldname = "";
  String where = "";

  Vector vReadDB = new Vector(0);

  public SecIndex2(String DBName) {
    dbname = DBName.trim();
  }

  //初始化信息源名,为空return true.
  public boolean isDatSrcNameEmpty() {
    if (datsrcname1.equalsIgnoreCase("")) {
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

  //
  public void parseDatSrcName() {
    int loc1 = datsrcname1.indexOf(",");
    while (loc1 >= 1) {
      vdatsrcname.addElement(datsrcname1.substring(0, loc1));
      String stmp = datsrcname1.substring(loc1 + 1);
      datsrcname1 = stmp;
      loc1 = datsrcname1.indexOf(",");
    }
    if (!datsrcname1.equalsIgnoreCase("")) {
      vdatsrcname.addElement(datsrcname1);
    }
  }

  //
  public void doIndex() {
    if (baseCheckBeforeRun()) { //基本设置对了，才进行
      final Vector vContent = new Vector(0);
      parseDatSrcName();
      String whnext = ConstantDef.TABLENAME + "='" +
          vdatsrcname.elementAt(0).toString() + "' and " +
          ConstantDef.SORTINDEX + ">0";
      ReadPDBTable readpdbtable = new ReadPDBTable();
      int recs = readpdbtable.getPDBPropwhere(whnext) ;
      if (recs > 0) { //初始化信息源名有索引
        searchedfieldname = readpdbtable.getAField(ConstantDef.SEARCHEDFIELDID ); //被检索域名
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
        if (!fieldname.equalsIgnoreCase("")) {
          pdll.queryAnyTable(dbname,
                             readpdbtable.getAField(ConstantDef.SEARCHEDTABLEID),
                             fieldname, where, orderfd, vContent, true);
          if (searchedfieldname.equalsIgnoreCase(showfieldname)) {
            vContent.add(1, (Vector) (vContent.elementAt(0)));
          }
        }
        String[] typefieldname = new String[1];
        typefieldname[0] = searchfieldname; //检索域名
        final int[] fieldtype = pdll.getFieldsType(dbname.trim(),
            vdatsrcname.elementAt(0).toString(), typefieldname); //检索域名数据类型

        final JButton button = new JButton("") {
          public void setText(String s) {
            //Button never shows text -- only color.
          }
        };
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

        final ReadDB readdb = new ReadDB(dbname.trim(),
                                     vdatsrcname.elementAt(0).toString().trim());

        final SecDialog dialog = new SecDialog(ApplicationDB.frameDB, "输入对话框", true);
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();

        JPanel jPanel1 = new JPanel();
        JButton jButtonCancle = new JButton();
        JButton jButtonOK = new JButton();
        GridLayout gridLayout1 = new GridLayout();
        BorderLayout borderLayout1 = new BorderLayout();
        final JComboBox comboBox = new JComboBox( (Vector) vContent.elementAt(1));
        comboBox.setSelectedIndex( -1);
        JScrollPane scrollPane = new JScrollPane();
        final DefaultListModel listModel = new DefaultListModel();
        final JList jlist = new JList(listModel);
        jlist.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        dialog.getContentPane().setLayout(borderLayout1);
        jPanel1.setMinimumSize(new Dimension(35, 35));
        jPanel1.setPreferredSize(new Dimension(35, 35));
        jPanel1.setLayout(gridLayout1);
        jButtonCancle.setPreferredSize(new Dimension(35, 55));
        jButtonOK.setPreferredSize(new Dimension(35, 55));
        comboBox.setMaximumSize(new Dimension(32767, 25));
        comboBox.setMinimumSize(new Dimension(122, 25));
        comboBox.setPreferredSize(new Dimension(126, 25));
        dialog.getContentPane().add(scrollPane, BorderLayout.CENTER);
        dialog.getContentPane().add(jPanel1, BorderLayout.SOUTH);
        scrollPane.getViewport().add(jlist, null);
      //  JTextField index = new JTextField();
        jPanel1.add(jButtonOK, null);
        jPanel1.add(jButtonCancle, null);
        jButtonOK.setText("确定");
        jButtonCancle.setText("取消");
        dialog.getContentPane().add(comboBox, BorderLayout.NORTH);
        dialog.setSize(screenSize.width / 8, screenSize.height / 4);

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
              readdb.queryTable(fd.trim(), we.trim(), "");
              listModel.removeAllElements();
              jlist.removeAll() ;
              if (readdb.getFields()  == 1) { //只有1个域时
                for (int mm = 0; mm < readdb.getRecords() ; mm++) {
                  listModel.addElement(readdb.getAFieldValue(mm,0) );
                }
              }
              else {
                for (int mm = 0; mm < readdb.getRecords() ; mm++) {
                    listModel.addElement(readdb.getAFieldValue(mm,1));//
                }
              }

              vReadDB = new Vector(0);

              for (int i = 1; i < vdatsrcname.size(); i++) {
                ReadDB readdb1 = new ReadDB(dbname.trim(),
                                        vdatsrcname.elementAt(i).toString().
                                        trim());
                vReadDB.add(readdb1) ;
                readdb1.queryTable(fd.trim(), we.trim(), "");
                if (readdb1.getFields() == 1) { //只有1个域时
                  for (int mm = 0; mm < readdb1.getRecords() ; mm++) {
                    listModel.addElement(readdb1.getAFieldValue(mm,0));
                  }
                }
                else {
                  for (int mm = 0; mm < readdb1.getRecords(); mm++) {
                      listModel.addElement(readdb1.getAFieldValue(mm,1));
                  }
                }
              }
            }
          }
        });

        //Here's the code that brings up the dialog.
        button.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e) {
            dialog.setLocationRelativeTo(button);
            dialog.show();
            //在此改变内容
            if (readdb.getFields()  == 1) { //不同时有列表域名和信息域名
              if (jlist.getSelectedIndex() >= 0 && dialog. getCancelF () == false) {
                siEditor.currentString = jlist.getSelectedValue();
              }
            }
            else {
              int jlistindex = jlist.getSelectedIndex();
              if (jlistindex >= 0 && dialog.getCancelF() == false) {
                if(jlistindex < readdb.getRecords() )
                  {
                  siEditor.currentString = readdb.getAFieldValue(jlistindex,0);
                  }
                  else
                  {
                    int count = readdb.getRecords() ;
                    for(int j = 0; j < vReadDB.size() ; j++)
                      {
                        ReadDB readdbtmp = (ReadDB)vReadDB.elementAt(j) ;
                        if(jlistindex < (count + readdbtmp.getRecords() ))
                           {
                             siEditor.currentString = readdbtmp.getAFieldValue(jlistindex - count,0);
                             break;
                           }
                           else
                             {
                               count += readdbtmp.getRecords() ;
                             }
                      }
                  }
              }
            }
          }
        });
      }
    }
  }
}
