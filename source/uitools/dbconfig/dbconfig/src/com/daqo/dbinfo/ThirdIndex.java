package com.daqo.dbinfo;

import javax.swing.tree.DefaultMutableTreeNode;

import com.daqo.dbinfo.table.*;
import com.daqo.dbinfo.renderer.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//三次分类检索
public class ThirdIndex {
  String dbname;
  String datsrcname; //初始化信息源名
  String datsrclistname; //列表域名
  String searchedfield; //信息域名

  Dlllink pdll = Dlllink.getInstance();

  ComboBoxEx comboBox = null;

  public ThirdIndex(String DBName) {
    dbname = DBName;
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
    if (isListNameEmpty() || isSearchedFieldEmpty()) {
      return false;
    }
    return true;
  }

  //
  public boolean doIndex(DefaultMutableTreeNode node) {
    if (baseCheckBeforeRun()) { //基本设置对了，才进行
      String nodetablename = "";
      int level = node.getLevel();
      if (level == 2) {
        nodetablename = node.toString();
      }
      else if (level == 3) {
        nodetablename = node.getParent().toString();
      }
      else if (level == 4) {
        nodetablename = node.getParent().getParent().toString();
      }
      String where = ConstantDef.TABLENAME + "='" + nodetablename + "' and " +
          ConstantDef.SORTINDEX + ">0";
      String wh = "";
      ReadPDBTable readpdbtable = new ReadPDBTable();
      int recs = readpdbtable.getPDBPropwhere(where) ;
      if (recs > 0) { //有分类索引
        String searedtable =readpdbtable.getAField(ConstantDef.SEARCHEDTABLEID ); //被检索表名
        if (searedtable.length() > 0) { //有被检索表名
          String whnext = ConstantDef.TABLENAME + "='" + searedtable + "' and " +
              ConstantDef.SORTINDEX + ">0";
          ReadPDBTable readpdbtable2 = new ReadPDBTable();
          int recs2 = readpdbtable2.getPDBPropwhere(whnext) ;

          //得到 被检索表名的 分类检索
          if (recs2 > 0) { //被检索表名有索引
            String wherethird = ConstantDef.TABLENAME + "='" + datsrcname +
                "' and " + ConstantDef.SORTINDEX + ">0";
            ReadPDBTable readpdbtable3 = new ReadPDBTable();
            int recs3 = readpdbtable3.getPDBPropwhere(wherethird) ;
            //要求初始化信息源名必须与显示表的第二级索引用相同的检索方式
            if (recs3 > 0) {
              String searchedfieldthird = readpdbtable3.getAField(ConstantDef.
                  SEARCHFIELDID );
              if (!searchedfieldthird.equalsIgnoreCase("")) {
                String searchtablename = readpdbtable3.getAField(ConstantDef.
                    SEARCHEDTABLEID);
                //第三次检索的SEARCHEDTABLE要与显示表的第二次索引一样
                if (searchtablename.equalsIgnoreCase(readpdbtable2.getAField(
                    ConstantDef.SEARCHEDTABLEID))) {
                  int[] objdatatype = new int[1];
                  byte[][] buflast;
                  //把jtrre上的节点描述转换成实际的检索域的值
                  buflast = pdll.getAMatchFieldValue(dbname,
                      readpdbtable2.getAField(ConstantDef.
                                  SEARCHEDTABLEID ),
                      readpdbtable2.getAField(ConstantDef.SEARCHEDFIELDID),
                      readpdbtable2.getAField(ConstantDef.SHOWFIELDID),
                      node.getParent().toString().trim(), objdatatype);
                  if (buflast.length > 0) {
                    if (objdatatype[0] == ConstantDef.DATATYPE) {
                      wh = searchedfieldthird + "=" +
                          Integer.parseInt(pdll.substr(buflast[0]));
                    }
                    else if (objdatatype[0] == ConstantDef.FLOATTYPE) {
                      wh = searchedfieldthird + "=" +
                          Float.parseFloat(pdll.substr(buflast[0]));
                    }
                    else if (objdatatype[0] == ConstantDef.STRINGTYPE) {
                      wh = searchedfieldthird + "='" + pdll.substr(buflast[0]) +
                          "'";
                    }
                  }
                }
              }
            }
          }
          else { //没有 被检索表的分类检索，用自己的分类检索
            wh = "";
          }
        }
      }
      //开始第三级索引
      comboBox = new ComboBoxEx();
      ReadDB readdb = new ReadDB(dbname, datsrcname);
      String fieldname3;
      boolean singlefieldname = true;
      if (searchedfield.equalsIgnoreCase("")) { //信息域名为空
        fieldname3 = datsrclistname;
      }
      else if (datsrclistname.equalsIgnoreCase("")) { //列表域名为空
        fieldname3 = searchedfield;
      }
      else {
        if (datsrclistname.equalsIgnoreCase(searchedfield)) {
          fieldname3 = datsrclistname;
        }
        else {
          fieldname3 = datsrclistname + "," + searchedfield; //列表域名 + 信息域名
          singlefieldname = false;
        }
      }
      readdb.queryTable(fieldname3.trim(), wh.trim(), "");
      int count = readdb.getRecords() ;
      String tmpStr = "",tmpStr1 = "";
      for (int n = 0; n < count; n++) {
        tmpStr = (String) readdb.getAFieldValue(n,0).trim()  ;
        comboBox.addItem(tmpStr); //列表域名
        comboBox.vDisp.add(n, tmpStr);
        if (readdb.getFields() == 1) {
         comboBox.vReal.add(tmpStr);
        }
        else {
          tmpStr1 = (String) readdb.getAFieldValue(n,1).trim()  ;
          comboBox.vReal.add(tmpStr1);
        }
      } //for n
      comboBox.setEditable(true);
      return true;
    } //baseCheckBeforeRun
    return false;
  }

}
