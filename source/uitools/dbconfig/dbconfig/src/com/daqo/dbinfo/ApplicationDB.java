package com.daqo.dbinfo;

import javax.swing.UIManager;
import java.awt.*;
import javax.swing.JComponent;
import java.util.Collections;
import javax.swing.DefaultFocusManager;
import java.io.IOException;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class ApplicationDB {
  boolean packFrame = false;
  public static FrameDB frameDB=null;

  //Construct the application
  public ApplicationDB() {
    FrameDB frame = new FrameDB();
    frameDB=frame;
    //frame.jTabbedPane1.setFocusTraversalKeys(KeyboardFocusManager.FORWARD_TRAVERSAL_KEYS, Collections.EMPTY_SET);
    //Validate frames that have preset sizes
    //Pack frames that have useful preferred size info, e.g. from their layout
    //DefaultFocusManager.disableSwingFocusManager();
    if (packFrame) {
      frame.pack();
    }
    else {
      frame.validate();
    }
    //Center the window
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    Dimension frameSize = frame.getSize();
    frameSize.height = screenSize.height - 30;
    frameSize.width = screenSize.width;
    frame.setLocation(0, 0);
    frame.setSize(frameSize);
    frame.setVisible(true);
    frame.userstatusbar.setText("无登录用户");
  }

  //Main method
  public static void main(String[] args) throws IOException {
    if(args == null || args.length == 0){
    }
    else{
      if (args[0].equals("--version")) {
        System.out.println(Param.getInstance().getVersion());
        return;
      }
      if (args[0].equals("--path")) {
        System.out.println(System.getProperty("java.library.path"));
        return;
      }
    }
    //洪祥 添加读取dbconfig.ini配置文件 --start
    Dlllink pdll = Dlllink.getInstance();
    Dlllink.runhome = new String(pdll.getenvvar()); //得到环境变量
    String filename=  Dlllink.runhome + "/resource/xml/dbconfig.xml";
    try {
      String Md5value = ConfigurationFile.getProfileString(filename, "Option",
                                                  "md5", "false");
      String lockvalue = ConfigurationFile.getProfileString(filename, "Option",
                                                  "lock", "false");
      String version = ConfigurationFile.getProfileString(filename, "Option",
                                                  "version", "1.3.0");
       if (Md5value.toLowerCase().equals("true")) {
         PreProcessor.MD5 = true;
       }
       if (lockvalue.toLowerCase().equals("true")) {
         PreProcessor.TableLock = true;
       }
       PreProcessor.vesion = version;
    }
    catch (IOException e) {
      System.out.println(e.toString());
    }


    //洪祥 添加读取dbconfig.ini配置文件 --end
    SecDialog secDB = new SecDialog(frameDB, "用户登录", true);
    FrameDB_AboutBox aboutdb = new FrameDB_AboutBox(frameDB);

    UserLogin dialog = new UserLogin(frameDB, "用户登录", true);
    TextFileViewer filedialog = new TextFileViewer(frameDB, "操作记录", true);
    dialog.setDefaultLookAndFeelDecorated(true);
    secDB.setDefaultLookAndFeelDecorated(true);
    aboutdb.setDefaultLookAndFeelDecorated(true);
    frameDB.setDefaultLookAndFeelDecorated(true);
    filedialog.setDefaultLookAndFeelDecorated(true);
    try {

      //根据操作系统设置外观属性 add by hongxiang 2016.8.15
      String osName = System.getProperties().getProperty("os.name"); //获得操作系统名称
      if (osName.startsWith("Windows")){
        UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
      }else {
        UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
      }

      Toolkit.getDefaultToolkit().addAWTEventListener(new
          java.awt.event.
          AWTEventListener() {

        boolean firstInputEvent = true;

        public void eventDispatched(AWTEvent event) {

          if (firstInputEvent) {

            firstInputEvent = false;

            frameDB.setDefaultLookAndFeelDecorated(false);

            final java.awt.event.AWTEventListener listener = this;

            javax.swing.SwingUtilities.invokeLater(new Runnable() {

              public void run() {

                frameDB.setDefaultLookAndFeelDecorated(true);

                Toolkit.getDefaultToolkit().

                    removeAWTEventListener(listener);

              }

            });

          }

        }

      }, AWTEvent.INPUT_METHOD_EVENT_MASK);
      // 一下这段代码解决的是使用LookAndFeel后引起JTextArea不能输入中文的BUG

      //---------------
    }
    catch (Exception e) {
      e.printStackTrace();
    }

    new ApplicationDB();
    User user = ConstantDef.getUserInstance();
    user.initUser();
    user.setBrowserPara(1);

  }
}
class Param{
  private static Param instance;
  private Param(){
  }
  public static Param getInstance(){
    if(instance==null) instance=new Param();
    return instance;
  }
  public String getVersion(){
    return PreProcessor.vesion;
  }
}
