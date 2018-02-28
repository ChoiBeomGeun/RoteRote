using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ControlDesignMode;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json;

namespace RoteRoteMapView
{
    public partial class MainView : Form
    {
        List<Control> objectList = new List<Control>();
        List<RoteObjects> RoteObjectlist = new List<RoteObjects>();
        int objectindex = 0;
        JSONTEST jsontest = new JSONTEST();
        
        enum ControlNodeType
        {
            Player, Wall, Button, Box, Trigger90, Trigger180, Clearzone, Hazard, Camera
        };

        public MainView()
        {
            
            JObject videogameRatings =
    new JObject(
        new JProperty("channel",
            new JObject(
                new JProperty("title",new JObject(new JProperty("rr","r"), new JProperty("rrr", "r"))),
                new JProperty("link", "http://james.newtonking.com"),
                new JProperty("description", "James Newton-King's blog."),
                new JProperty("item", 1)
          
                   )));




            File.WriteAllText("videogames.json", videogameRatings.ToString());


            JObject o1 = JObject.Parse(File.ReadAllText("videogames.json"));
            var postTitles =
      from p in o1["channel"]["title"]["rr"]
      select (string)p["title"];


            //Console.WriteLine(o1.ToString());
            InitializeComponent();
            this.WindowState = FormWindowState.Maximized;
            this.chkDesignMode.Checked = this.designModePanel.IsDesignMode;
            this.CreateTreeControlNodes();
            this.chkDesignMode.CheckedChanged += chkDesignMode_CheckedChanged;
            this.InitDesignModePanel();


        }

        /// <summary>
        /// 데모 컨트롤 추가
        /// </summary>
        private void InitDesignModePanel()
        {
           
            this.designModePanel.Controls.Add(new Button() { Text = "Test", Location = new Point(10, 10) });
 
            this.designModePanel.MouseRightClick += drawPanel1_MouseRightClick;
            this.designModePanel.MouseLeftClick += drawPanel1_MouseLeftClick;
            this.designModePanel.Painted += designModePanel_Painted;
        }

        /// <summary>
        /// 디자인모드 패널이 컨트롤들을 다 그리고나서 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void designModePanel_Painted(object sender, PaintEventArgs e)
        {
      
            if (designModePanel.IsDesignMode)
            {
              
                using (Pen pen = new Pen(Brushes.Red, 10))
                {
                    Rectangle rect = this.designModePanel.ClientRectangle;
                    rect.Inflate(new Size(-50, -50));
                    e.Graphics.DrawRectangle(pen, rect);
                }
            }
        }

        /// <summary>
        /// 컨트롤이 마우스 클릭되면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        void drawPanel1_MouseRightClick(object sender, MouseDownEventArgs args)
        {
            
           // propertyGrid1.SelectedObject = tempobject;
            args.Control.BringToFront();
            propertyGrid1.SelectedObject = args.Control;
              
        }
        void drawPanel1_MouseLeftClick(object sender, MouseDownEventArgs args)
        {
           
           // args.Control.BringToFront();
            propertyGrid1.SelectedObject = args.Control;
            
            //     propertyGrid1.SelectedObject = args.Control;

        }
        /// <summary>
        /// 디자인 모드 변경시 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void chkDesignMode_CheckedChanged(object sender, EventArgs e)
        {
     
            this.designModePanel.IsDesignMode = this.chkDesignMode.Checked;
        }

        /// <summary>
        /// 컨트롤 노드들을 생성한다.
        /// </summary>
        private void CreateTreeControlNodes()
        {
            string[] controlNames = Enum.GetNames(typeof(ControlNodeType));
            for (int i = 0; i < controlNames.Length; i++)
            {
                
                treeControls.Nodes.Add(controlNames[i]);
            }
        }

        /// <summary>
        /// TreeView에서 Drag가 시작되면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void treeControls_ItemDrag(object sender, ItemDragEventArgs e)
        {
          
            TreeNode selectedNode = (e.Item as TreeNode);
            Control control = null;
            RoteObjects tempobject = null;
            ControlNodeType nodeType;
            if (Enum.TryParse<ControlNodeType>(selectedNode.Text,out nodeType))
            {
                switch (nodeType)
                {
                    case ControlNodeType.Player:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./player.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            objectList.Add(control);
                            propertyGrid1.SelectedObject = tempobject;

                        }
                        break;
                    case ControlNodeType.Wall:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./wall.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            objectList.Add(control);
                            propertyGrid1.SelectedObject = tempobject;

                        }
                        break;
                    case ControlNodeType.Button:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            objectList.Add(control);
                            propertyGrid1.SelectedObject = tempobject;

                        }
                        break;
                     case ControlNodeType.Box:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./box.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            objectList.Add(control);
                            propertyGrid1.SelectedObject = tempobject;

                        }
                        break;
                    case ControlNodeType.Trigger180:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./180button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            objectList.Add(control);
                            propertyGrid1.SelectedObject = tempobject;

                        }
                        break;
                    case ControlNodeType.Trigger90:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./90button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            objectList.Add(control);
                            propertyGrid1.SelectedObject = tempobject;

                        }
                        break;
                    case ControlNodeType.Hazard:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./hazard.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            objectList.Add(control);
                            propertyGrid1.SelectedObject = tempobject;

                        }
                        break;
                    case ControlNodeType.Camera:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./camera.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            objectList.Add(control);
                            propertyGrid1.SelectedObject = tempobject;

                        }
                        break;
                    case ControlNodeType.Clearzone:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./clearzone.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            objectList.Add(control);
                            propertyGrid1.SelectedObject = tempobject;

                        }
                        break;
                }
              

            }
            selectedNode.Tag = control;

           
            DoDragDrop(e.Item, DragDropEffects.Move);
         
        }

        private void designModePanel_Paint(object sender, PaintEventArgs e)
        {
          //  propertyGrid1.SelectedObject = objectLists.SelectedItem;
            
        }

        private void propertyGrid1_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            if (e.ChangedItem.Label == "PageDelay")
            {
                object value = e.ChangedItem.Value;

                MessageBox.Show(value.ToString());


            }
            else if (e.ChangedItem.Label == "Capture")
            {

                object value = e.ChangedItem.Value;

                MessageBox.Show(value.ToString());

            }
        }

        private void objectLists_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            objectLists.DisplayMember = "name";
            objectLists.DisplayMember = null;
           // propertyGrid1.SelectedObject = objectLists.SelectedItem;
        }

        private void propertyGrid1_Click(object sender, EventArgs e)
        {

        }

        private void designModePanel_MouseClick(object sender, MouseEventArgs e)
        {
            propertyGrid1.SelectedObject = objectLists.SelectedItem;
        }

        private void designModePanel_MouseDoubleClick(object sender, MouseEventArgs e)
        {

        }

        private void splitContainer1_Panel2_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
