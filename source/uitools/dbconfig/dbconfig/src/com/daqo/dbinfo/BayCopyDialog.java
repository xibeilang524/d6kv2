package com.daqo.dbinfo;

import com.borland.jbcl.layout.XYConstraints;
import com.borland.jbcl.layout.XYLayout;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.util.ArrayList;

/**
 * Created by 洪祥 on 2015/7/10.
 */
public class BayCopyDialog extends JDialog {

    ExecuteBayCopy executeBayCopy = new ExecuteBayCopy(this);

    JLabel sourceSub = new JLabel();
    JLabel targetSub = new JLabel();
    JLabel sourceTerminal = new JLabel();
    JLabel targetTerminal = new JLabel();
    JLabel YC = new JLabel();
    JLabel YX = new JLabel();
    JLabel KWH = new JLabel();
    //复制进度条
    JLabel process = new JLabel();
    JProgressBar progressbar = new JProgressBar();
    //站所复制下拉框
    JComboBox subCopy_from = new JComboBox();
    JComboBox subCopy_to = new JComboBox();
    //终端复制下拉框
    JComboBox terminalCopy_from = new JComboBox();
    JComboBox terminalCopy_to = new JComboBox();
    //选择点表
    JCheckBox checkBox_YC = new JCheckBox();
    JCheckBox checkBox_YX = new JCheckBox();
    JCheckBox checkBox_KWH = new JCheckBox();

    JButton jBtOK = new JButton();
    JButton jBtCancel = new JButton();
    XYLayout xYLayout1 = new XYLayout();

    public void init() throws Exception {

        xYLayout1.setWidth(286);
        xYLayout1.setHeight(323);
        this.setForeground(Color.pink);
        this.getContentPane().setLayout(xYLayout1);

        sourceSub.setText("源厂站：");
        targetSub.setText("目标厂站：");
        sourceTerminal.setText("源终端：");
        targetTerminal.setText("目标终端：");
        YC.setText("遥测参数：");
        YX.setText("遥信参数：");
        KWH.setText("电度参数：");

        progressbar.setOrientation(JProgressBar.HORIZONTAL);
        progressbar.setMinimum(0);
        progressbar.setMaximum(100);
        progressbar.setValue(0);
        progressbar.setStringPainted(true);
        progressbar.setPreferredSize(new Dimension(120, 20));
        progressbar.setBorderPainted(true);
        progressbar.setBackground(Color.lightGray);
        progressbar.setForeground(Color.cyan);
        progressbar.setVisible(false);

        //站所复制选项
        this.getContentPane().add(sourceSub, new XYConstraints(26, 12, 90, 26));
        this.getContentPane().add(targetSub, new XYConstraints(26, 48, 90, 26));
        this.getContentPane().add(subCopy_from, new XYConstraints(96, 12, 168, 26));
        this.getContentPane().add(subCopy_to, new XYConstraints(96, 48, 168, 26));
        //终端复制选项
        this.getContentPane().add(sourceTerminal, new XYConstraints(26, 98, 90, 26));
        this.getContentPane().add(targetTerminal, new XYConstraints(26, 134, 90, 26));
        this.getContentPane().add(terminalCopy_from, new XYConstraints(96, 98, 168, 26));
        this.getContentPane().add(terminalCopy_to, new XYConstraints(96, 134, 168, 26));
        //选择需要复制的点表
        this.getContentPane().add(YC, new XYConstraints(26, 176, 100, 26));
        this.getContentPane().add(checkBox_YC, new XYConstraints(100, 176, 28, 26));
        this.getContentPane().add(YX, new XYConstraints(26, 206, 100, 26));
        this.getContentPane().add(checkBox_YX, new XYConstraints(100, 206, 28, 26));
        this.getContentPane().add(KWH, new XYConstraints(26, 236, 100, 26));
        this.getContentPane().add(checkBox_KWH, new XYConstraints(100, 236, 28, 26));
        //复制进度
        this.getContentPane().add(process, new XYConstraints(136, 186, 120, 26));
        this.getContentPane().add(progressbar, new XYConstraints(136, 210, 126, 20));
        //取消或执行操作
        this.getContentPane().add(jBtCancel, new XYConstraints(28, 276, 96, 26));
        this.getContentPane().add(jBtOK, new XYConstraints(168, 276, 96, 26));

        //填充厂站下拉框
        fillComboBox();

        //源厂站选择项改变
        subCopy_from.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                String item = subCopy_from.getSelectedItem().toString();
                ArrayList<String> terminals = executeBayCopy.getTerminalBySub(item);
                terminalCopy_from.removeAllItems();
                for (String terminal : terminals){
                    byte[][] yc = executeBayCopy.getYCParaInfoByTerminal(terminal);
                    byte[][] yx = executeBayCopy.getYXParaInfoByTerminal(terminal);
                    byte[][] kwh = executeBayCopy.getKWHParaInfoByTerminal(terminal);
                    //非空终端作为源终端
                    if (yc.length != 0 || yx.length != 0 || kwh.length != 0){
                        terminalCopy_from.addItem(terminal);
                    }
                }
                //设置复选框
                setCheckBoxSelected();
            }
        });

        //目标厂站选择项改变
        subCopy_to.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                String item = subCopy_to.getSelectedItem().toString();
                ArrayList<String> terminals = executeBayCopy.getTerminalBySub(item);
                terminalCopy_to.removeAllItems();
                for (String terminal : terminals){
                    byte[][] yc = executeBayCopy.getYCParaInfoByTerminal(terminal);
                    byte[][] yx = executeBayCopy.getYXParaInfoByTerminal(terminal);
                    byte[][] kwh = executeBayCopy.getKWHParaInfoByTerminal(terminal);
                    //空的终端作为目标终端(遥测参数表、遥信参数表、电度参数表都为空)
                    if(yc.length == 0 && yx.length == 0 && kwh.length == 0){
                        terminalCopy_to.addItem(terminal);
                    }
                }
            }
        });

        jBtCancel.setText("退出");
        jBtCancel.addActionListener(new BayCopyDialog_jBtCancel_actionAdapter(this));
        jBtOK.setText("复制");
        jBtOK.addActionListener(new BayCopyDialog_jBtOK_actionAdapter(this));
    }

    //填充厂站下拉框
    private void fillComboBox(){

        subCopy_from.removeAllItems();
        subCopy_to.removeAllItems();
        terminalCopy_from.removeAllItems();
        terminalCopy_to.removeAllItems();

        ArrayList<String> sourceSub = executeBayCopy.getSourceSub();
        for(int i = 0; i < sourceSub.size(); i++){
            this.subCopy_from.addItem(sourceSub.get(i));
        }

        ArrayList<String> targetSub = executeBayCopy.getTargetSub();
        for(int i = 0; i < targetSub.size(); i++){
            this.subCopy_to.addItem(targetSub.get(i));
        }

        subCopy_from.setSelectedIndex( -1);
        subCopy_to.setSelectedIndex( -1);
    }

    //设置复选框
    private void setCheckBoxSelected(){
        checkBox_YC.setSelected(true);
        checkBox_YX.setSelected(true);
        checkBox_KWH.setSelected(true);
        checkBox_YC.setEnabled(false);
        checkBox_YX.setEnabled(false);
        checkBox_KWH.setEnabled(false);
    }

    public BayCopyDialog(Frame frame, String title, boolean modal) {
        super(frame, title, modal);
        try {
            setResizable(false);
            setBounds(300, 300, 300, 350);
            init();
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }

    }

    public void jBtOK_actionPerformed(ActionEvent e) {
        executeBayCopy.start();
    }

    public void jBtCancel_actionPerformed(ActionEvent e) {
        dispose();
    }

    class BayCopyDialog_jBtCancel_actionAdapter implements ActionListener {

        private BayCopyDialog bayCopyDialog;

        BayCopyDialog_jBtCancel_actionAdapter(BayCopyDialog bayCopyDialog) {
            this.bayCopyDialog = bayCopyDialog;
        }

        public void actionPerformed(ActionEvent e) {
            bayCopyDialog.jBtCancel_actionPerformed(e);
        }

    }

    class BayCopyDialog_jBtOK_actionAdapter implements ActionListener {

        private BayCopyDialog bayCopyDialog;

        BayCopyDialog_jBtOK_actionAdapter(BayCopyDialog bayCopyDialog) {
            this.bayCopyDialog = bayCopyDialog;
        }

        public void actionPerformed(ActionEvent e) {

            if(subCopy_from.getSelectedIndex() == -1 || subCopy_to.getSelectedIndex() == -1){
                JOptionPane.showMessageDialog(null, "请选择厂站！", "警告", JOptionPane.WARNING_MESSAGE);
            }else if(terminalCopy_from.getSelectedIndex() == -1 || terminalCopy_to.getSelectedIndex() == -1){
                JOptionPane.showMessageDialog(null, "请选择终端！", "警告", JOptionPane.WARNING_MESSAGE);
            }else {
                String subFrom = "源终端：" + this.bayCopyDialog.terminalCopy_from.getSelectedItem().toString();
                String terminalFrom = "目标终端：" + this.bayCopyDialog.terminalCopy_to.getSelectedItem().toString();

                int option = JOptionPane.showConfirmDialog(null,
                        subFrom + "；" + terminalFrom + "。确认复制？", "提示", JOptionPane.YES_NO_OPTION,
                        JOptionPane.WARNING_MESSAGE, null);

                if(option == JOptionPane.YES_OPTION){
                    bayCopyDialog.jBtOK_actionPerformed(e);
                }
            }
        }
    }
}