package usacreport;

import java.awt.event.*;
import java.awt.*;
import java.awt.print.*;
import javax.swing.*;

import java.awt.geom.*;

public class PrintPreviewDialog extends JDialog implements ActionListener
{
    private JButton nextButton = new JButton("Next");
    private JButton previousButton = new JButton("Previous");
    private JButton closeButton = new JButton("Close");
    private JPanel buttonPanel = new JPanel();
    private PreviewCanvas canvas;

    public PrintPreviewDialog(Frame parent, String title, boolean modal, Print pt, String str)
    {
        super(parent, title, modal);
        canvas = new PreviewCanvas(pt, str);
        setLayout();
    }

  /**
   * PrintPreviewDialog
   */
  public PrintPreviewDialog() {
  }

  private void setLayout()
    {
        this.getContentPane().setLayout(new BorderLayout());
        this.getContentPane().add(canvas, BorderLayout.CENTER);

        nextButton.setMnemonic('N');
        nextButton.addActionListener(this);
        buttonPanel.add(nextButton);
        previousButton.setMnemonic('N');
        previousButton.addActionListener(this);
        buttonPanel.add(previousButton);
        closeButton.setMnemonic('N');
        closeButton.addActionListener(this);
        buttonPanel.add(closeButton);
        this.getContentPane().add(buttonPanel, BorderLayout.SOUTH);
     //   this.setBounds((int)((SystemProperties.SCREEN_WIDTH - 800) / 2), (int)((SystemProperties.SCREEN_HEIGHT - 600) / 2), 800, 600);
    }

    public void actionPerformed(ActionEvent evt)
    {
        Object src = evt.getSource();
        if (src == nextButton)
            nextAction();
        else if (src == previousButton)
            previousAction();
        else if (src == closeButton)
            closeAction();
    }

    private void closeAction()
    {
        this.setVisible(false);
        this.dispose();
    }

    private void nextAction()
    {
        canvas.viewPage(1);
    }

    private void previousAction()
    {
        canvas.viewPage(-1);
    }

    class PreviewCanvas extends JPanel
    {
        private String printStr;
        private int currentPage = 0;
        private Print preview;

        public PreviewCanvas(Print pt, String str)
        {
            printStr = str;
            preview = pt;
        }

        public void paintComponent(Graphics g)
        {
            super.paintComponent(g);
            Graphics2D g2 = (Graphics2D)g;
            PageFormat pf = PrinterJob.getPrinterJob().defaultPage();

            double xoff;
            double yoff;
            double scale;
            double px = pf.getWidth();
            double py = pf.getHeight();
            double sx = getWidth() - 1;
            double sy = getHeight() - 1;
            if (px / py < sx / sy)
            {
                scale = sy / py;
                xoff = 0.5 * (sx - scale * px);
                yoff = 0;
            }
            else
            {
                scale = sx / px;
                xoff = 0;
                yoff = 0.5 * (sy - scale * py);
            }
            g2.translate((float)xoff, (float)yoff);
            g2.scale((float)scale, (float)scale);

            Rectangle2D page = new Rectangle2D.Double(0, 0, px+200, py+200);
            g2.setPaint(Color.white);
            g2.fill(page);
            g2.setPaint(Color.black);
            Font f=new Font("Arial",10,20);
            g2.setFont(f);
            g2.draw(page);
            try
            {
                preview.print(g2, pf, currentPage);
            }
            catch(PrinterException pe)
            {
                g2.draw(new Line2D.Double(0, 0, px, py));
                g2.draw(new Line2D.Double(0, px, 0, py));
            }
        }

        public void viewPage(int pos)
        {
            int newPage = currentPage + pos;
            if (0 <= newPage && newPage < preview.getPagesCount(printStr))
            {
                currentPage = newPage;
                repaint();
            }
        }
    }

}
