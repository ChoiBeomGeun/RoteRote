using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
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
        public Main()
        {
            InitializeComponent();
        }
        
        private void Form1_Load(object sender, EventArgs e)
        {
         
            listBox1.Items.Add("1920 X 1080");
            listBox1.Items.Add("1600 X 900");
            listBox1.Items.Add("1280 X 960");
            listBox1.Items.Add("1024 X 768");
            listBox1.Items.Add("800 X 600");
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex != -1)
            {
                using (StreamWriter rdr = new StreamWriter(Environment.CurrentDirectory + "\\temp.ini"))
                {
                    rdr.WriteLine(listBox1.SelectedItem.ToString());
                    rdr.WriteLine(checkBox1.Checked.ToString());
                }

                System.Diagnostics.Process ps = new System.Diagnostics.Process();
                ps.StartInfo.FileName = "examples.exe";
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
            JObject Reading = null;
 
                Reading = JObject.Parse(File.ReadAllText("levels.\\" +"levelClearInfo.json"));

            int LevelNumber = System.Int16.Parse(Reading["NumberOfTheLevels"].ToString());
            for (int i = 1; i <= LevelNumber; i++)
            {
                string Level = "level" + i.ToString() + ".json";

                Reading[Level] = true;



            }
            Reading["level" + 1.ToString() + ".json"] = false;
            File.WriteAllText("levels.\\" +"levelClearInfo.json", Reading.ToString());


        }
    }
}
