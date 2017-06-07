package com.daqo.dbinfo.table;

import java.io.*;
import java.util.StringTokenizer;
import java.util.Vector;
import java.io.RandomAccessFile;

import com.daqo.dbinfo.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */
//读文件,文件的每行分别放在Vector中
public class ReadFile {

  private String fileName; //文件名
  public Vector vlist = new Vector(0); //文件行记录
  public Vector vindexlist = new Vector(0); //vindexlist存放文件索引
  public Vector vindexcontent = new Vector(0); //存放包括索引在内的文件的一行完整记录
  Dlllink pdll = Dlllink.getInstance();

  public ReadFile() {
  }

  public ReadFile(String filename) {
    fileName = filename;
  }


public void setFileName(String filename)
    {
      this.fileName = filename;
    }

  public void openFile(boolean indexf) { //打开文件时，indexf=true找出每行的索引值，否则就不找
   // byte[][] contentbuf = pdll.querydbinfoini("项目名= '" + fileName + "'"); //优先读数据库中的设置
  //  if (contentbuf.length <= 0) { //数据库中没有设置，就读文件     //广东测试 删除掉  hx
  if (-1 <= 0) {
      int loc = fileName.lastIndexOf(".txt");
      String filepath;
      if (loc < 0) {
        filepath = Dlllink.runhome + "/" + ConstantDef.DBINFOPATH + "/" +
            fileName + ".txt";
      }
      else {
        filepath = Dlllink.runhome + "/" + ConstantDef.DBINFOPATH + "/" +
            fileName;
      }
      try {
        File fileobject = new File(filepath);
        int filelen = (int) fileobject.length();
        byte[] buf = new byte[filelen];
        InputStream fileIn = new FileInputStream(filepath);
        fileIn.read(buf);
        String s = new String(buf);
        StringTokenizer t = new StringTokenizer(s, "\r\n", false);
        String stmp;
        while (t.hasMoreTokens()) {
          stmp = t.nextToken().trim();
          if (stmp.trim().equalsIgnoreCase("") == false) {
            vlist.addElement(stmp);
            if (indexf) { //有找出索引的标记
              int loc1 = stmp.indexOf("*");
              if (loc1 >= 1) {
                vindexlist.addElement(stmp.substring(0, loc1));
                vindexcontent.addElement(stmp);
              }
            }
          }
        }
        fileIn.close();
      }
      catch (java.io.IOException IOE) {

      }
    } //if(contentbuf.length <=0)
    else {
  /*
      String stmp;
      for (int i = 0; i < contentbuf.length; i++) {
        stmp = pdll.substr(contentbuf[i]);
        vindexlist.removeAllElements();
        if (stmp.trim().equalsIgnoreCase("") == false) {
          vlist.addElement(stmp);
          if (indexf) { //有找出索引的标记
            int loc1 = stmp.indexOf("*");
            if (loc1 >= 1) {
              vindexlist.addElement(stmp.substring(0, loc1));
              vindexcontent.addElement(stmp);
            }
          }
        }
      }
    */
    }
  }

  //
  public void openFileNew() {
    byte[][] contentbuf = pdll.querydbinfoini("项目名='" + fileName + "'"); //优先读数据库中的设置
    if (contentbuf.length <= 0) { //数据库中没有设置，就读文件
      int loc = fileName.lastIndexOf(".txt");
      String filepath;
      if (loc < 0) {
        filepath = Dlllink.runhome + "\\" + ConstantDef.DBINFOPATH + "\\" +
            fileName + ".txt";
      }
      else {
        filepath = Dlllink.runhome + "\\" + ConstantDef.DBINFOPATH + "\\" +
            fileName;
      }
      try {
        File fileobject = new File(filepath);
        int filelen = (int) fileobject.length();
        byte[] buf = new byte[filelen];
        InputStream fileIn = new FileInputStream(filepath);
        fileIn.read(buf);
        String s = new String(buf);
        StringTokenizer t = new StringTokenizer(s, "\r\n", false);
        String stmp;
        vindexlist.removeAllElements();
        while (t.hasMoreTokens()) {
          stmp = t.nextToken().trim();
          vlist.addElement(stmp);
          vindexlist.addElement(Integer.toString(vindexlist.size()));
          vindexcontent.addElement(stmp);
        }
        fileIn.close();
      }
      catch (java.io.IOException IOE) {

      }
    } //if(contentbuf.length <=0)
    else {
      String stmp;
      vindexlist.removeAllElements();
      for (int i = 0; i < contentbuf.length; i++) {
        stmp = pdll.substr(contentbuf[i]);
        vlist.addElement(stmp);
        vindexlist.addElement(Integer.toString(vindexlist.size()));
        vindexcontent.addElement(stmp);
      }
    }
  }
}
