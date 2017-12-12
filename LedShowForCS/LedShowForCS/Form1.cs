using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Runtime.InteropServices;
using LedShowForCS.Properties;
using System.Threading;
using System.Messaging;

namespace LedShowForCS
{

    public partial class Form1 : Form
    {
        [DllImport("user32.dll")]
        public static extern bool ReleaseCapture();
        [DllImport("user32.dll")]
        public static extern bool SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);
        public const int WM_SYSCOMMAND = 0x0112;
        public const int SC_MOVE = 0xF010;
        public const int HTCAPTION = 0x0002;

        public string strLedShowText = "这里是测试滚动文字的标签，您可以修改此文字来显示屏幕内容。";

        const int HTLEFT = 10;
        const int HTRIGHT = 11;
        const int HTTOP = 12;
        const int HTTOPLEFT = 13;
        const int HTTOPRIGHT = 14;
        const int HTBOTTOM = 15;
        const int HTBOTTOMLEFT = 0x10;
        const int HTBOTTOMRIGHT = 17;

        bool bLeftMode = true;
        string strFont;
        int nFontSize;
        int nSpeed;
        int nLoopNum;
        int nBackColor;
        int nTextColor;
        PointF CurrentPos;
        Font TextFont;
        Color cBackColor;
        Brush cTextColor;
        string temp;
        int nCurrentPlayTime = 1;
        int nInfoNum = 0;
        MessageQueue myqueue = null;
        List<string> strShowList;
        public Form1()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.None; //加上此句，不显示标题栏
        }
        private void RecvMsg()
        {
            MessageQueue mq = new MessageQueue(@".\private$\CdmLedC");

            mq.Formatter = new XmlMessageFormatter(new Type[] { typeof(string) });
            Thread th = new Thread(() =>
            {
                while (true)
                {
                    System.Messaging.Message msg = mq.Receive();
                    if (msg != null)
                    {
                        //MessageBox.Show(msg.Body.ToString());
                        if(strShowList == null)
                        {
                            strShowList = new List<string>();
                        }
                        if (strShowList.Count >= nInfoNum)
                        {
                            strShowList.RemoveAt(0);
                        }
                        strShowList.Add(msg.Body.ToString());
                        //strLedShowText = msg.Body.ToString();
                    }

                }
            });
            th.IsBackground = true;
            th.Start();
        }
        private void Form1_Load(object sender, EventArgs e)
        {

            IniFiles TestIni = new IniFiles("./Ledshow.ini");
            strFont = TestIni.ReadString("Setting", "Font", "");
            nFontSize = TestIni.ReadInteger("Setting", "FontSize", 0);
            nSpeed = TestIni.ReadInteger("Setting", "PlaySpeed", 0);
            nLoopNum = TestIni.ReadInteger("Setting", "PlayTime", 0);
            bLeftMode = TestIni.ReadBool("Setting", "LeftMode", false);
            nInfoNum = TestIni.ReadInteger("Setting", "ShowInfoNum", 0);
            nBackColor = TestIni.ReadInteger("Color", "BackColor", 0);
            nTextColor = TestIni.ReadInteger("Color", "TextColor", 0);
            TextFont = new Font(strFont, nFontSize);
            cBackColor = SetBackColor(nBackColor);
            cTextColor = SetTextColor(nTextColor);
            if (Settings.Default.WindowLocation != null)
            {
                this.Location = Settings.Default.WindowLocation;
            }
            if (Settings.Default.WindowSize != null)
            {
                this.Size = Settings.Default.WindowSize;
            }
            this.timer1.Enabled = true;
            if (nSpeed > 10)
                nSpeed = 10;
            if (nSpeed < 1)
                nSpeed = 1;
            this.timer1.Interval = 500 / nSpeed;
 
            string queuepath = @".\private$\CdmLedC";
            if (!MessageQueue.Exists(queuepath))
            {
                myqueue = MessageQueue.Create(queuepath);
            }
            myqueue = new MessageQueue(queuepath);
//             if(strShowList == null)
//             {
//                 strShowList = new List<string>();
//                 strShowList.Add("这是测试多条消息存储在队列后在显示出来的文字");
//             }
            RecvMsg();
            CurrentPos = new PointF(this.Size.Width, 50);
        }
        private Brush SetTextColor(int nColor)
        {
            Brush retBrush = Brushes.Red;
            switch (nColor)
            {
                case 0:
                    retBrush = Brushes.Black;
                    break;
                case 1:
                    retBrush = Brushes.White;
                    break;
                case 2:
                    retBrush = Brushes.Red;
                    break;
                case 3:
                    retBrush = Brushes.Green;
                    break;
                case 4:
                    retBrush = Brushes.Blue;
                    break;
                case 5:
                    retBrush = Brushes.Yellow;
                    break;
                case 6:
                    retBrush = Brushes.Cyan;
                    break;
                case 7:
                    retBrush = Brushes.Orange;
                    break;
                case 8:
                    retBrush = Brushes.Purple;
                    break;
                default:
                    retBrush = Brushes.Black;
                    break;
            }
            return retBrush;
        }
        private Color SetBackColor(int nColor)
        {
            Color retColor = Color.Black;
            switch(nColor)
            {
                case 0:
                    retColor = Color.Black;
                    break;
                case 1:
                    retColor = Color.White;
                    break;
                case 2:
                    retColor = Color.Red;
                    break;
                case 3:
                    retColor = Color.Green;
                    break;
                case 4:
                    retColor = Color.Blue;
                    break;
                case 5:
                    retColor = Color.Yellow;
                    break;
                case 6:
                    retColor = Color.Cyan;
                    break;
                case 7:
                    retColor = Color.Orange;
                    break;
                case 8:
                    retColor = Color.Purple;
                    break;
                default:
                    retColor = Color.Black;
                    break;
            }
                
            return retColor;
        }
        private List<string> GetStringRows(Graphics graphic, Font font, string text, int width)
        {
            int RowBeginIndex = 0;
            int rowEndIndex = 0;
            int textLength = text.Length;
            List<string> textRows = new List<string>();
            float fRate = (nFontSize>10)?(1+(float)1.5*((float)nFontSize/(float)20)):1;
            for (int index = 0; index < textLength; index++)
            {
                rowEndIndex = index;
                float nWidth = graphic.MeasureString(text.Substring(RowBeginIndex, rowEndIndex - RowBeginIndex + 1), font).Width;
                if (index == textLength - 1)
                {
                    textRows.Add(text.Substring(RowBeginIndex));
                }
                else if (rowEndIndex + 1 < text.Length && text.Substring(rowEndIndex, 2) == "\r\n")
                {
                    textRows.Add(text.Substring(RowBeginIndex, rowEndIndex - RowBeginIndex));
                    rowEndIndex = index += 2;
                    RowBeginIndex = rowEndIndex;
                }
                else if (graphic.MeasureString(text.Substring(RowBeginIndex, rowEndIndex - RowBeginIndex + 1), font).Width *fRate> width)
                {
                    textRows.Add(text.Substring(RowBeginIndex, rowEndIndex - RowBeginIndex));
                    RowBeginIndex = rowEndIndex;
                }
            }

            return textRows;
        }
        private void GetShowText()
        {
            strLedShowText = "";
            if (strShowList == null)
            {
                strLedShowText = "";
                return;
            }
            for(int i=0;i<strShowList.Count;i++)
            {
                strLedShowText += strShowList[i];
                if(bLeftMode)
                    strLedShowText += "\t";
                else
                    strLedShowText += "\r\n";
            }
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            Graphics g = CreateGraphics();//this.label1.CreateGraphics();
            SizeF s = new SizeF();
            GetShowText();
            s = g.MeasureString(strLedShowText, TextFont);//测量文字长度  
            string strDraw = "";
            if (!bLeftMode)
            {
                List<string> textRows = GetStringRows(g, this.Font, this.strLedShowText, this.Size.Width);
                //自动折行处理,目前还存在问题
                foreach (string str in textRows)
                {
                    strDraw += str + "\n";
                }
            }
            else
            {
                strDraw = strLedShowText;
            }
            if (nCurrentPlayTime > nLoopNum)
            {
                strDraw = "";
            }
            g.Clear(cBackColor);//背景颜色  

            if (temp != strLedShowText)//文字改变时,重新显示  
            {
                if (bLeftMode)
                    CurrentPos = new PointF(this.Size.Width, 0);
                else
                    CurrentPos = new PointF(0, this.Size.Height);
                temp = strLedShowText;
                nCurrentPlayTime = 1;
            }
            else
            {
                if (bLeftMode)
                    CurrentPos = new PointF(CurrentPos.X - 20, 0);//每次偏移10  
                else
                    CurrentPos = new PointF(0, CurrentPos.Y - 10);
            }
            if (bLeftMode)
            {
                if (CurrentPos.X <= -s.Width)
                {
                    CurrentPos = new PointF(this.Size.Width, 0);
                    nCurrentPlayTime++;
                }
            }
            else
            {
                if (CurrentPos.Y <= -s.Height * 1.5)
                {
                    CurrentPos = new PointF(0, this.Size.Height);
                    nCurrentPlayTime++;
                }
            }
            g.DrawString(strDraw, TextFont, cTextColor, CurrentPos);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Settings.Default.WindowLocation = this.Location;
            if (this.WindowState == FormWindowState.Normal)
            {
                Settings.Default.WindowSize = this.Size;
            }
            else
            {
                Settings.Default.WindowSize = this.RestoreBounds.Size;
            }
            if (this.WindowState != FormWindowState.Minimized)
                Settings.Default.Save();//使用Save方法保存更改
        }

        protected override void WndProc(ref System.Windows.Forms.Message m)
        {
            switch (m.Msg)
            {
                case 0x0084:
                    base.WndProc(ref m);
                    Point vPoint = new Point((int)m.LParam & 0xFFFF,
                        (int)m.LParam >> 16 & 0xFFFF);
                    vPoint = PointToClient(vPoint);
                    if (vPoint.X <= 5)
                        if (vPoint.Y <= 5)
                            m.Result = (IntPtr)HTTOPLEFT;
                        else if (vPoint.Y >= ClientSize.Height - 5)
                            m.Result = (IntPtr)HTBOTTOMLEFT;
                        else m.Result = (IntPtr)HTLEFT;
                    else if (vPoint.X >= ClientSize.Width - 5)
                        if (vPoint.Y <= 5)
                            m.Result = (IntPtr)HTTOPRIGHT;
                        else if (vPoint.Y >= ClientSize.Height - 5)
                            m.Result = (IntPtr)HTBOTTOMRIGHT;
                        else m.Result = (IntPtr)HTRIGHT;
                    else if (vPoint.Y <= 5)
                        m.Result = (IntPtr)HTTOP;
                    else if (vPoint.Y >= ClientSize.Height - 5)
                        m.Result = (IntPtr)HTBOTTOM;
                    break;
                case 0x0201://鼠标左键按下的消息 
                    m.Msg = 0x00A1;//更改消息为非客户区按下鼠标 
                    m.LParam = IntPtr.Zero;//默认值 
                    m.WParam = new IntPtr(2);//鼠标放在标题栏内 
                    base.WndProc(ref m);
                    break;
                default:
                    base.WndProc(ref m);
                    break;
            }
        }
        static int n = 0;
        private void Form1_MouseDown(object sender, MouseEventArgs e)
        {
            string s = "客户端往队列里发送的信息";
            s += n.ToString();
            n++;
            System.Messaging.Message msg = new System.Messaging.Message();
            msg.Body = s;
            msg.Formatter = new XmlMessageFormatter(new Type[] { typeof(string) });
            MessageQueue mq = new MessageQueue(@"FormatName:Direct=TCP:192.168.8.12\private$\CdmLedC");
            mq.Send(msg);
//             if(bLeftMode)
//                 strLedShowText = "这是改变后的滚动测试文本；\t现在是第二条显示的滚动文本内容";
//             else
//                 strLedShowText = "这是改变后的滚动测试文本；\r\n\r\n现在是第二条显示的滚动文本内容";
        }
    }
}
