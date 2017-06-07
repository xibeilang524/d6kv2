package usacreport;

import javax.swing.UIManager;
import java.awt.*;

/**
 * Description: 报表工具
 * Company: daqo
 * version 1.0
 */

public class AppSacReport {
  private boolean packFrame = false;
  public static String s_cmdline = "";
  public static FrmSacReport frameDB=null;
  //Construct the application
  public AppSacReport() {
    FrmSacReport frame = new FrmSacReport();
    frameDB=frame;
    //Validate frames that have preset sizes
    //Pack frames that have useful preferred size info, e.g. from their layout
    if (packFrame) {
      frame.pack();
    }
    else {
      frame.validate();
    }
    //Center the window
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    Dimension frameSize = frame.getSize();
    if (frameSize.height > screenSize.height) {
      frameSize.height = screenSize.height;
    }
    if (frameSize.width > screenSize.width) {
      frameSize.width = screenSize.width;
    }
    frame.setLocation((screenSize.width - frameSize.width) / 2, (screenSize.height - frameSize.height) / 2);
    frame.setVisible(true);
    //javax.swing.ImageIcon t=new javax.swing.ImageIcon(usacreport.FrmSacReport.class.getResource("icon/my_p2.gif"));
    //frame.setIconImage(t.getImage());
    //初始化预处理器对象
    new PreProcessor();
  }



  //Main method
  public static void main(String[] args) {
    try {
       if (args == null || args.length == 0) {
       }
       else {
          if (args[0].equals("--version")) {
             System.out.println(Param.getInstance().getVersion());
             return;
          }
       }

    //  UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
//   UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");

        //根据操作系统设置外观属性 add by hongxiang 2016.11.25
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
      System.out.println("open report!");
      if(args.length>0){
         s_cmdline = args[0].toString().trim();
      }
    }
    catch(Exception e) {
      e.printStackTrace();
    }
    new AppSacReport();
  }
}
class Param{
  private static Param instance;
  public final String version="1.0";
  private Param(){
  }
  public static Param getInstance(){
    if(instance==null) instance=new Param();
    return instance;
  }
  public String getVersion(){
    return version;
  }
}
