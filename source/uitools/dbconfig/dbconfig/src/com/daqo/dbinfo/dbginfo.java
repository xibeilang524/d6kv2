package com.daqo.dbinfo;

import java.io.File;
import java.io.IOException;
import java.io.*;
import java.io.OutputStream;
import java.io.FileNotFoundException;
import java.util.*;
import javax.swing.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//jhongtu ++ 20060117
public class dbginfo {

  public dbginfo() {
  }

//tag,一项的标识
//logmsg，一项内容
  static  public  void writelog( String tag ,String logmsg)
  {
    String filename = Dlllink.runhome + "/log/dbconfiglog.txt";

    File  file = new File(filename);

    long filelength =  file.length() ;

    if(filelength > (1024*1024*12))//4M
    {
      file.delete() ;
    }

    if(file.exists() ==false)
    {
      try{
        file.createNewFile() ;
        }catch(IOException ioe)
        {
        //  System.out.println("dbinfo.java");
          ioe.printStackTrace() ;
        }
    }

    try{

      FileOutputStream fos = new FileOutputStream(file ,true);

      PrintWriter pw = new PrintWriter(fos);

      Calendar calendar = Calendar.getInstance() ;

      calendar.set(Calendar.ZONE_OFFSET, 28800000);

      int a = calendar.get(Calendar.AM_PM);
      int hour = calendar.get(Calendar.HOUR );

      if(a == 1)//下午
      {
        hour += 12;
      }

      int mon = calendar.get(Calendar.MONTH ) + 1;
      String  date = calendar.get(Calendar.YEAR) + "-" + Integer.toString(mon)   + "-" + Integer.toString(calendar.get(Calendar.DATE))
                   + "  " + Integer.toString(hour) + ":" + Integer.toString(calendar.get(Calendar.MINUTE))
                   + ":" + Integer.toString(calendar.get(Calendar.SECOND)) + "     " + tag + "  " + logmsg ;
      pw.println(date);
      pw.close();

      }catch(Exception fne)
      {
        fne.printStackTrace();
      }
  }
}
