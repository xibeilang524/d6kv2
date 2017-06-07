package com.daqo.dbinfo;

import com.borland.jbcl.layout.XYConstraints;
import com.borland.jbcl.layout.XYLayout;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

/**
 * Created by 洪祥 on 2015/4/8.
 * 数据库相关表重复性校验时的进度条
 */
public class VerifyProgressBar extends JDialog {


    JLabel label;
    JProgressBar progressbar;
    JButton jBtOK = new JButton();
    JButton jBtCancel = new JButton();
    XYLayout xYLayout1 = new XYLayout();

    public VerifyProgressBar(Frame frame, String title, boolean modal) {
        super(frame, title, modal);
        try {
            setResizable(false);
            setBounds(300, 300, 300, 150);
            init();
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private void init(){

        xYLayout1.setWidth(286);
        xYLayout1.setHeight(100);
        this.setForeground(Color.pink);
        this.getContentPane().setLayout(xYLayout1);

        label = new JLabel("点击确定开始校验");

        progressbar = new JProgressBar();
        progressbar.setOrientation(JProgressBar.HORIZONTAL);
        progressbar.setMinimum(0);
        progressbar.setMaximum(100);
        progressbar.setValue(0);
        progressbar.setStringPainted(true);
        progressbar.setPreferredSize(new Dimension(300, 20));
        progressbar.setBorderPainted(true);
        progressbar.setBackground(Color.lightGray);
        progressbar.setForeground(Color.cyan);

        this.getContentPane().add(label, new XYConstraints(16, 10, 160, 26));
        this.getContentPane().add(progressbar, new XYConstraints(12, 40, 268, 20));
        this.getContentPane().add(jBtOK, new XYConstraints(28, 76, 86, 26));
        this.getContentPane().add(jBtCancel, new XYConstraints(168, 76, 86, 26));

        jBtCancel.setText("取消");
        jBtCancel.addActionListener(new VerifyProgressBar_jBtCancel_actionAdapter(this));
        jBtOK.setText("确定");
        jBtOK.addActionListener(new VerifyProgressBar_jBtOK_actionAdapter(this));
    }

    public void setButtonEnabled(boolean enabled) {
        jBtOK.setEnabled(enabled);
    }

    public void setBarText(String str) {
        label.setText(str);
        label.setForeground(Color.blue);
    }

    public void setBarValue(int value){
        progressbar.setValue(value);
    }

    class VerifyProgressBar_jBtOK_actionAdapter implements ActionListener {

        private VerifyProgressBar verifyProgressBar;

        VerifyProgressBar_jBtOK_actionAdapter(VerifyProgressBar verifyProgressBar) {
            this.verifyProgressBar = verifyProgressBar;
        }

        public void actionPerformed(ActionEvent e) {
            verifyProgressBar.jBtOK_actionPerformed(e);
        }
    }

    public void jBtOK_actionPerformed(ActionEvent e) {

        ExecuteVerifyProcess executeVerifyProcess = new ExecuteVerifyProcess(this);
        executeVerifyProcess.start();
    }

    class VerifyProgressBar_jBtCancel_actionAdapter implements ActionListener {

        private VerifyProgressBar verifyProgressBar;

        VerifyProgressBar_jBtCancel_actionAdapter(VerifyProgressBar verifyProgressBar) {
            this.verifyProgressBar = verifyProgressBar;
        }

        public void actionPerformed(ActionEvent e) {
            verifyProgressBar.jBtCancel_actionPerformed(e);
        }

    }

    public void jBtCancel_actionPerformed(ActionEvent e) {
        dispose();
    }
}




