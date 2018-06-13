/******************************************************************************/
/*!
\file   Main.cs
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    RoteRoteMapEditor
\date   2018/6/6

Main view of Launcher
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Management;
using System.Diagnostics;
using System.IO;
using Newtonsoft.Json.Linq;
using RoteMapView;

namespace RoteRoteLauncherView
{
    public partial class Main : Form
    {
        int i_hiddencount = 0;
        bool b_IsHiddenFormAppeared = true;
        private Size MaxSize;
        private Size []CurrentSize = new Size[6];
        private Size ClickedSize;
        public Main()
        {
            InitializeComponent();
            MaxSize = MonitorInfoCSharp.Form1.returnMaxSize();

            
        }
        
        private void Form1_Load(object sender, EventArgs e)
        {
         
            listBox1.Items.Add("1920 X 1080");
            listBox1.Items.Add("1680 X 1050");
            listBox1.Items.Add("1600 X 900");
            listBox1.Items.Add("1280 X 960");
            listBox1.Items.Add("1280 X 720");
            listBox1.Items.Add("1024 X 768");
            
            CurrentSize[0] = new Size (1920, 1080);
            CurrentSize[1] = new Size(1680, 1050);
            CurrentSize[2] = new Size(1600, 900);
            CurrentSize[3] = new Size(1280, 960);
            CurrentSize[4] = new Size(1280, 720);
            CurrentSize[5] = new Size(1024, 768);
            listBox1.SelectedIndex = 0;
        }

        private void button1_Click(object sender, EventArgs e)
        {

            string temp = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            temp += ".\\RoteRote.\\temp.ini";
            if (listBox1.SelectedIndex != -1)
            {
                if (ClickedSize.Width > MaxSize.Width ||
                    ClickedSize.Height > MaxSize.Height)
                {
                    DialogResult result = MessageBox.Show("Your Monitor doesn't support this Resolution\n " +
                                                          "Will you play the game with recommended Resolution("
                                                          + listBox1.Items[++listBox1.SelectedIndex].ToString() + ")",
                        "Rresolution ERROR",
                        MessageBoxButtons.YesNo);

                    if (result == DialogResult.No)
                    {
                        return;
                    }

             
                }


                using (StreamWriter rdr = new StreamWriter(temp))
                {
                    rdr.WriteLine(listBox1.SelectedItem.ToString());
                    rdr.WriteLine(checkBox1.Checked.ToString());

                    if (checkBox2.Checked)
                        rdr.WriteLine("KOR");
                    else
                        rdr.WriteLine("ENG");
                }

                System.Diagnostics.Process ps = new System.Diagnostics.Process();
                ps.StartInfo.FileName = "RoteRote.exe";
                ps.Start();
                Application.Exit();
            }
        }

    

        private void pictureBox1_Click(object sender, EventArgs e)
        {
     
            i_hiddencount++;
 
            if (i_hiddencount>5 &&b_IsHiddenFormAppeared)
            {
                this.Size = new System.Drawing.Size(500, 280);
                this.MaximumSize = new System.Drawing.Size(500, 280);
                this.MinimumSize = new System.Drawing.Size(500, 280);
                button3.Location = new Point(345, 12);
                button3.Visible = true;

                button2.Location = new Point(345, 70);
                button2.Visible = true;
                b_IsHiddenFormAppeared = false;
                i_hiddencount = 0;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            ArcheTypeEditorForm ATE = new ArcheTypeEditorForm();
            ATE.ShowDialog();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            RoteMapView.MainView mainView = new RoteMapView.MainView();
            mainView.ShowDialog();
        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {
            string temp = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            temp += ".\\RoteRote.\\ClearInfo.json";
            JObject Reading = null;
 
                Reading = JObject.Parse(File.ReadAllText(temp));

            int LevelNumber = System.Int16.Parse(Reading["NumberOfTheLevels"].ToString());
            for (int i = 1; i <= LevelNumber; i++)
            {
                string Level = "level" + i.ToString() + ".json";

                Reading[Level] = true;


           
            }
            Reading["level" + 1.ToString() + ".json"] = false;
            File.WriteAllText(temp, Reading.ToString());


        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ClickedSize = CurrentSize[listBox1.SelectedIndex];

        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
