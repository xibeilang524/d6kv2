package com.daqo.dbinfo;

import com.borland.jbcl.layout.XYConstraints;
import com.borland.jbcl.layout.XYLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.ArrayList;

/**
 * Created by 洪祥 on 2016/7/20.
 */
public class DevCopyDialog extends JDialog {

    ExecuteDevCopy executeDevCopy = new ExecuteDevCopy(this);

    JLabel sourceSub = new JLabel();
    JLabel sourceDev = new JLabel();

    JLabel targetSub = new JLabel();
    JLabel targetTerminal = new JLabel();
    JLabel targetDev = new JLabel();

    //设备复制源下拉框
    JComboBox subCopy_from = new JComboBox(); //源长站
    JComboBox devCopy_from = new JComboBox(); //源设备

    //设备复制目标下拉框
    JComboBox subCopy_to = new JComboBox(); //目标厂站
    JComboBox terminalCopy_to = new JComboBox(); //目标终端
    JComboBox devCopy_to = new JComboBox(); //目标设备

    //单设备复制
    JLabel singleDevCopy = new JLabel();
    JCheckBox checkBox_singleDevCopy = new JCheckBox();

    //全设备复制
    JLabel allDevCopy = new JLabel();
    JCheckBox checkBox_allDevCopy = new JCheckBox();

    //复制进度条
    JLabel process = new JLabel();
    JProgressBar progressbar = new JProgressBar();


    JButton jBtOK = new JButton();
    JButton jBtCancel = new JButton();
    XYLayout xYLayout1 = new XYLayout();

    public void init() throws Exception {

        xYLayout1.setWidth(286);
        xYLayout1.setHeight(323);
        this.setForeground(Color.pink);
        this.getContentPane().setLayout(xYLayout1);

        sourceSub.setText("源厂站：");
        sourceDev.setText("源设备：");

        targetSub.setText("目标厂站：");
        targetTerminal.setText("目标终端：");
        targetDev.setText("目标设备：");

        singleDevCopy.setText("单设备复制：");
        allDevCopy.setText("全设备复制：");

        process.setText("复制进度：");
        progressbar.setOrientation(JProgressBar.HORIZONTAL);
        progressbar.setMinimum(0);
        progressbar.setMaximum(100);
        progressbar.setValue(0);
        progressbar.setStringPainted(true);
        progressbar.setPreferredSize(new Dimension(120, 20));
        progressbar.setBorderPainted(true);
        progressbar.setBackground(Color.lightGray);
        progressbar.setForeground(Color.cyan);
        progressbar.setVisible(true);

        //设备复制选项
        this.getContentPane().add(sourceSub, new XYConstraints(26, 12, 90, 26));
        this.getContentPane().add(sourceDev, new XYConstraints(26, 48, 90, 26));
        this.getContentPane().add(subCopy_from, new XYConstraints(96, 12, 168, 26));
        this.getContentPane().add(devCopy_from, new XYConstraints(96, 48, 168, 26));
        //终端复制选项
        this.getContentPane().add(targetSub, new XYConstraints(26, 96, 90, 26));
        this.getContentPane().add(targetTerminal, new XYConstraints(26, 132, 90, 26));
        this.getContentPane().add(targetDev, new XYConstraints(26, 168, 90, 26));
        this.getContentPane().add(subCopy_to, new XYConstraints(96, 96, 168, 26));
        this.getContentPane().add(terminalCopy_to, new XYConstraints(96, 132, 168, 26));
        this.getContentPane().add(devCopy_to, new XYConstraints(96, 168, 168, 26));
        //设置设备复制模式
        this.getContentPane().add(singleDevCopy, new XYConstraints(36, 206, 100, 26));
        this.getContentPane().add(checkBox_singleDevCopy, new XYConstraints(110, 206, 28, 26));
        this.getContentPane().add(allDevCopy, new XYConstraints(156, 206, 100, 26));
        this.getContentPane().add(checkBox_allDevCopy, new XYConstraints(238, 206, 28, 26));
        //复制进度
        this.getContentPane().add(process, new XYConstraints(26, 236, 120, 26));
        this.getContentPane().add(progressbar, new XYConstraints(96, 240, 168, 20));
        //取消或执行操作
        this.getContentPane().add(jBtCancel, new XYConstraints(28, 276, 96, 26));
        this.getContentPane().add(jBtOK, new XYConstraints(168, 276, 96, 26));

        //填充厂站下拉框
        fillComboBox();

        //源厂站选择项改变
        subCopy_from.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                if (subCopy_from.getSelectedIndex() != -1){
                    String item = subCopy_from.getSelectedItem().toString();
                    //非空设备作为源设备
                    ArrayList<String> sourceDevList = executeDevCopy.getNotEmptyDev(item);
                    devCopy_from.removeAllItems();
                    for (int i = 0; i < sourceDevList.size(); i++){
                        devCopy_from.addItem(sourceDevList.get(i));
                    }
                    devCopy_from.setSelectedIndex(-1);
                    setCheckBoxSelected();
                }
            }
        });

        //源设备选择项改变
        devCopy_from.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(ItemEvent e) {

                if (devCopy_from.getSelectedIndex() != -1 && subCopy_to.getSelectedIndex() != -1){
                    devCopy_to.removeAllItems();
                    String sub = subCopy_to.getSelectedItem().toString();
                    String type = executeDevCopy.getDevTypeBySubAndDevDesc(subCopy_from.getSelectedItem().toString(), devCopy_from.getSelectedItem().toString());
                    //获取目标厂站中所有和源设备同类型的空设备作为目标设备
                    ArrayList<String> targetDevList = executeDevCopy.getEmptyDevOfSameType(sub, type);
                    for (int i = 0; i < targetDevList.size(); i++){
                        devCopy_to.addItem(targetDevList.get(i));
                    }
                }
                devCopy_to.setSelectedIndex(-1);
            }
        });

        //目标厂站选择项改变
        subCopy_to.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                if (subCopy_to.getSelectedIndex() != -1){
                    String item = subCopy_to.getSelectedItem().toString();
                    ArrayList<String> terminalList = executeDevCopy.getTerminalListBySub(item);
                    String type = executeDevCopy.getDevTypeBySubAndDevDesc(subCopy_from.getSelectedItem().toString(),devCopy_from.getSelectedItem().toString());
                    //获取目标厂站中所有和源设备同类型的空设备作为目标设备
                    ArrayList<String> targetDevList = executeDevCopy.getEmptyDevOfSameType(item, type);
                    terminalCopy_to.removeAllItems();
                    devCopy_to.removeAllItems();
                    //设置目标终端下拉框
                    for (int i = 0; i < terminalList.size(); i++){
                        terminalCopy_to.addItem(terminalList.get(i));
                    }
                    //设置目标设备下拉框
                    for (int i = 0; i < targetDevList.size(); i++){
                        devCopy_to.addItem(targetDevList.get(i));
                    }
                    //如果目标厂站对应的终端数目大于1，那么目标终端下拉框Index设为-1，由用户选择终端
                    if (terminalList.size() > 1){
                        terminalCopy_to.setSelectedIndex(-1);
                    }
                    devCopy_to.setSelectedIndex(-1);
                    setCheckBoxSelected();
                }
            }
        });

        //单设备复制和全设备复制勾选互斥
        checkBox_singleDevCopy.addItemListener(new ItemListener() {

            public void itemStateChanged(ItemEvent e) {

                if (checkBox_singleDevCopy.isSelected()) {
                    checkBox_allDevCopy.setSelected(false);
                } else {
                    checkBox_allDevCopy.setSelected(true);
                }

            }
        });

        //单设备复制和全设备复制勾选互斥
        checkBox_allDevCopy.addItemListener(new ItemListener() {

            public void itemStateChanged(ItemEvent e) {

                if (checkBox_allDevCopy.isSelected()) {
                    checkBox_singleDevCopy.setSelected(false);
                } else {
                    checkBox_singleDevCopy.setSelected(true);
                }

            }
        });

        jBtCancel.setText("退出");
        jBtCancel.addActionListener(new BayCopyDialog_jBtCancel_actionAdapter(this));
        jBtOK.setText("复制");
        jBtOK.addActionListener(new BayCopyDialog_jBtOK_actionAdapter(this));
    }

    //恢复页面初始状态
    public void restoreInterface(){
        subCopy_from.setSelectedIndex(-1);
        devCopy_from.setSelectedIndex(-1);
        subCopy_to.setSelectedIndex(-1);
        terminalCopy_to.setSelectedIndex(-1);
        devCopy_to.setSelectedIndex(-1);
        subCopy_from.setEnabled(true);
        devCopy_from.setEnabled(true);
        subCopy_to.setEnabled(true);
        terminalCopy_to.setEnabled(true);
        devCopy_to.setEnabled(true);
        checkBox_singleDevCopy.setEnabled(true);
        checkBox_singleDevCopy.setSelected(false);
        checkBox_allDevCopy.setEnabled(true);
        checkBox_allDevCopy.setSelected(false);
        process.setText("复制进度：");
        process.setForeground(Color.BLACK);
        progressbar.setValue(0);
        jBtCancel.setEnabled(true);
        jBtOK.setEnabled(true);
        executeDevCopy = new ExecuteDevCopy(this); //复制完后进程重新初始化
    }

    //填充厂站下拉框
    private void fillComboBox(){

        subCopy_from.removeAllItems();
        devCopy_from.removeAllItems();
        subCopy_to.removeAllItems();
        terminalCopy_to.removeAllItems();
        devCopy_to.removeAllItems();

        ArrayList<String> sourceSub = executeDevCopy.getSourceSub();
        for (int i = 0; i < sourceSub.size(); i++){
            this.subCopy_from.addItem(sourceSub.get(i));
        }

        ArrayList<String> targetSub = executeDevCopy.getTargetSub();
        for(int i = 0; i < targetSub.size(); i++){
            this.subCopy_to.addItem(targetSub.get(i));
        }

        subCopy_from.setSelectedIndex( -1);
        subCopy_to.setSelectedIndex( -1);
    }

    //设置设备复制模式复选框
    private void setCheckBoxSelected(){

        //一个厂站只对应一个终端的情况
        if (terminalCopy_to.getItemCount() == 1){
            checkBox_singleDevCopy.setSelected(false); //闭锁单设备复制
            checkBox_allDevCopy.setSelected(true); //复制整个厂站的同类型设备点表
            checkBox_singleDevCopy.setEnabled(false);
            checkBox_allDevCopy.setEnabled(false);
//            devCopy_to.setEnabled(false);
        }else if (terminalCopy_to.getItemCount() > 1){
            //一个厂站有多个终端的情况，只执行单个设备复制，由人来控制目标设备所属的终端
            checkBox_singleDevCopy.setSelected(true); //单设备复制
            checkBox_allDevCopy.setSelected(false); //闭锁整个厂站的设备复制
            checkBox_singleDevCopy.setEnabled(false);
            checkBox_allDevCopy.setEnabled(false);
//            devCopy_to.setEnabled(true);
        }else {
            //恢复初始化状态
            checkBox_singleDevCopy.setSelected(false);
            checkBox_allDevCopy.setSelected(false);
            checkBox_singleDevCopy.setEnabled(true);
            checkBox_allDevCopy.setEnabled(true);
            devCopy_to.setEnabled(true);
        }
    }

    public DevCopyDialog(Frame frame, String title, boolean modal) {
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
        if(subCopy_from.getSelectedIndex() == -1 || subCopy_to.getSelectedIndex() == -1){   //源厂站或目标厂站未选择
            JOptionPane.showMessageDialog(null, "请选择厂站！", "警告", JOptionPane.WARNING_MESSAGE);
        }else if (devCopy_from.getSelectedIndex() == -1){   //目标设备未选择
            JOptionPane.showMessageDialog(null, "请选择源设备！", "警告", JOptionPane.WARNING_MESSAGE);
        }else if (terminalCopy_to.getSelectedIndex() == -1){    //目标终端未选择
            JOptionPane.showMessageDialog(null, "请选择终端！", "警告", JOptionPane.WARNING_MESSAGE);
        }else if (devCopy_to.getItemCount() == 0){     //目标设备下拉框为空
            JOptionPane.showMessageDialog(null, "没有符合条件的目标设备！", "警告", JOptionPane.WARNING_MESSAGE);
        }else if (terminalCopy_to.getSelectedIndex() != -1
                && terminalCopy_to.getItemCount() > 1
                && devCopy_to.getSelectedIndex() == -1){    //一个厂站对应多个终端的情况，需要人为选择目标设备
            JOptionPane.showMessageDialog(null, "请选择目标设备！", "警告", JOptionPane.WARNING_MESSAGE);
        }else {
            executeDevCopy.start();
        }
    }

    public void jBtCancel_actionPerformed(ActionEvent e) {
        dispose();
    }

    class BayCopyDialog_jBtCancel_actionAdapter implements ActionListener {

        private DevCopyDialog bayCopyDialog;

        BayCopyDialog_jBtCancel_actionAdapter(DevCopyDialog bayCopyDialog) {
            this.bayCopyDialog = bayCopyDialog;
        }

        public void actionPerformed(ActionEvent e) {
            bayCopyDialog.jBtCancel_actionPerformed(e);
        }

    }

    class BayCopyDialog_jBtOK_actionAdapter implements ActionListener {

        private DevCopyDialog bayCopyDialog;

        BayCopyDialog_jBtOK_actionAdapter(DevCopyDialog bayCopyDialog) {
            this.bayCopyDialog = bayCopyDialog;
        }

        public void actionPerformed(ActionEvent e) {
            bayCopyDialog.jBtOK_actionPerformed(e);
        }
    }
}