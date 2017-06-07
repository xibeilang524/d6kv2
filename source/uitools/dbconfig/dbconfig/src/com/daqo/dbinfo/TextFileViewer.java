package com.daqo.dbinfo;
import java.awt.Frame;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import javax.swing.JDialog;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.*;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;

public class TextFileViewer
    extends JDialog {
  JTextArea   textarea   =   new   JTextArea(40,60);
  JScrollPane   scroller   =   new   JScrollPane(   textarea   );

  String filename;
  Dlllink pdll = Dlllink.getInstance();
  // 构造方法，创建用来显示指定目录中的指定文件内容的TextFileViewer对象
  public TextFileViewer(Frame owner, String title, boolean modal) {
    super(owner, title, modal);
  scroller.setHorizontalScrollBarPolicy(
      JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
  scroller.setVerticalScrollBarPolicy(
      JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);


    this.add(scroller);
    pack();

    //---------------------
      filename=Dlllink.runhome + "/log/" +
             "dbconfiglog.txt";
         textarea.setText(""); // 清除文本区域
         textarea.setLineWrap(true);

         try {
           File fileobject = new File(filename);
           int filelen = (int) fileobject.length();
           byte[] buf = new byte[filelen];
           InputStream fileIn = new FileInputStream(filename);
           fileIn.read(buf);
           String s = new String(buf);
           if(filelen != -1){
             textarea.setText(s);
           }
           fileIn.close();
         }
         catch (java.io.IOException e) {
           textarea.setText(e.getClass().getName() + ":" + e.getMessage());
           this.setTitle("操作记录: " +  filename + ":I/O Exception");

       }

   //---------------------
   // 指明目录
   // 当用户请求时销毁窗口
   addWindowListener(new WindowAdapter() {
     public void windowClosing(WindowEvent e) {
       dispose();
     }
   });

  }

}
