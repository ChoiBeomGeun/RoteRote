using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using ControlDesignMode;
using System.IO;
using System.Windows.Forms.VisualStyles;

namespace RoteMapView
{



    public partial class MainView : Form
    {
       public bool IsThereClearZone = false;
       public bool IsThereCamera = false;
       public bool IsTherePlayer = false;

        public bool IsMakingMovingWall = false;
        List<Control> objectList = new List<Control>();
        List<RoteObject> RoteobjectList = new List<RoteObject>();
        List<RoteObject> PathsList = new List<RoteObject>();
        RoteObject roCamera = new RoteObject();
        public int NumberOfPaths = 1;
        public int NumberOfMovingWalls = 1;
        public int TotalPaths = 0;
        enum ControlNodeType
        {
            Player, AttachWall, Button, Box,AttachBox,
            Trigger90_0, Trigger90_90, Trigger90_180,Trigger90_270,
            Trigger90Right_0, Trigger90Right_90, Trigger90Right_180, Trigger90Right_270,
            Trigger180_0, Trigger180_90, Trigger180_180,Trigger180_270,
            limited3Trigger90_0, limited3Trigger90_90, limited3Trigger90_180, limited3Trigger90_270,
            limited3Trigger180_0, limited3Trigger180_90, limited3Trigger180_180, limited3Trigger180_270,
            Clearzone, Hazard, Camera,MovingWall,Wall
        };
        
        public MainView()
        {
            InitializeComponent();
            this.WindowState = FormWindowState.Maximized;
            this.chkDesignMode.Checked = this.designModePanel.IsDesignMode;
            this.CreateTreeControlNodes();
            this.chkDesignMode.CheckedChanged += chkDesignMode_CheckedChanged;
            this.InitDesignModePanel();
            textBox2.ScrollBars = ScrollBars.Vertical;
            this.KeyPreview = true;



        }

        /// <summary>
        /// 데모 컨트롤 추가
        /// </summary>
        private void InitDesignModePanel()
        {

         //   this.designModePanel.Controls.Add(new Button() { Text = "Test", Location = new Point(10, 10) });
       
            this.designModePanel.MouseRightClick += drawPanel1_MouseRightClick;
            this.designModePanel.MouseLeftClick += drawPanel1_MouseLeftClick;
            this.designModePanel.Painted += designModePanel_Painted;

            //this.designModePanel.Controls.Clear();
        }

        /// <summary>
        /// 디자인모드 패널이 컨트롤들을 다 그리고나서 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void designModePanel_Painted(object sender, PaintEventArgs e)
        {
         
        }

        public Size GetSizeOfLastMovingWall()
        { 
            for (int i = RoteobjectList.Count; i > 0; i--)
            {
            
                 if (RoteobjectList[i-1].objectstyle == "MovingWall")
                {

                    return new Size((int)(RoteobjectList[i-1].ScaleX), (int)(RoteobjectList[i-1].ScaleY));
                }


            }
            return new Size(20, 20);
        }

        public Image RotateImage(Image img, RotateFlipType Rotation)
        {
            var bmp = new Bitmap(img);

            using (Graphics gfx = Graphics.FromImage(bmp))
            {
                gfx.Clear(Color.White);
                gfx.DrawImage(img, 0, 0, img.Width, img.Height);
            }

            bmp.RotateFlip(Rotation);
            return bmp;
        }
        public void DeleteUnvisibleItem()
        {

            SynchronizationObjects();
            int index = 0;

            int preCount = RoteobjectList.Count;

            RoteobjectList.RemoveAll(item => ((item.PositionX == 0)&& (item.PositionY==0)));

            if (preCount != RoteobjectList.Count)
            {
                foreach (var vItems in RoteobjectList)
                {

                    vItems.ObjectID = index;
                    index++;


                }

            }
        }
        public void SynchronizationObjects()
        {
            for(int i=0; i < this.designModePanel.Controls.Count;i++)
            {
                if (this.designModePanel.Controls[i].Tag == null ||
                    this.designModePanel.Controls[i].Tag.ToString() == "Camera" 
                   )
                    continue;

                int ObjectID = Int32.Parse(this.designModePanel.Controls[i].Tag.ToString());
                this.designModePanel.Controls[i].Refresh();
            
                RoteobjectList[ObjectID].PositionX = this.designModePanel.Controls[i].Location.X;
                RoteobjectList[ObjectID].PositionY = this.designModePanel.Controls[i].Location.Y;
                RoteobjectList[ObjectID].ScaleX = this.designModePanel.Controls[i].Size.Width;
                RoteobjectList[ObjectID].ScaleY = this.designModePanel.Controls[i].Size.Height;



            }



      


        }
        /// <summary>
        /// 컨트롤이 마우스 클릭되면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        /// 


        void drawPanel1_MouseRightClick(object sender, MouseDownEventArgs args)
        {
            if (propertyGrid1.SelectedObject == null)
                return;
            if (args.Control != null && args.Control.Tag != null)
            {

                RoteObject temp = (RoteObject)(propertyGrid1.SelectedObject);
                Point tempPoint = new Point((int)(temp.PositionX), (int)temp.PositionY);
                Control temp2 = null;
             
                for (int i = 0; i < this.designModePanel.Controls.Count; i++)
                {
                    if (this.designModePanel.Controls[i].Tag != null &&
                        (temp.ObjectID == Int32.Parse(this.designModePanel.Controls[i].Tag.ToString())))
                    {
                        if (temp.objectstyle == "MovingWall")
                        {

                            NumberOfMovingWalls++;
                            NumberOfPaths = 1;
                            IsMakingMovingWall = false;
                        }
                    }

                }


            }

            args.Control.BringToFront();

           
        }
        void drawPanel1_MouseLeftClick(object sender, MouseDownEventArgs args)
        {

          
            JsonTextUpdate();
    
            if (args.Control != null&& args.Control.Tag != null)
            { 
          
                if (args.Control.Tag.ToString() == "Camera")
                {
                    roCamera.PositionX = args.Control.Location.X;
                    roCamera.PositionY = args.Control.Location.Y;
                    propertyGrid1.SelectedObject = roCamera;


                    return;
                }
                if (args.Control.Tag.ToString() == "Path" || RoteobjectList.Count < Int32.Parse(args.Control.Tag.ToString()))
                    return;
                RoteobjectList[Int32.Parse(args.Control.Tag.ToString())].PositionX = args.Control.Location.X;
                RoteobjectList[Int32.Parse(args.Control.Tag.ToString())].PositionY = args.Control.Location.Y;
                RoteobjectList[Int32.Parse(args.Control.Tag.ToString())].ScaleX = args.Control.Size.Width;
                RoteobjectList[Int32.Parse(args.Control.Tag.ToString())].ScaleY = args.Control.Size.Height;
               
                propertyGrid1.SelectedObject = RoteobjectList[Int32.Parse(args.Control.Tag.ToString())];

               
               

            }

           // DeleteUnvisibleItem();
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
                this.treeControls.Nodes.Add(controlNames[i]);
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
            
            ControlNodeType nodeType;
            if (Enum.TryParse<ControlNodeType>(selectedNode.Text, out nodeType))
            {
                

                switch (nodeType)
                {
                    case ControlNodeType.Player:
                    {
                        if (IsMakingMovingWall)
                        {
                            MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;     
                        }
                            if (IsTherePlayer)
                            {
                                MessageBox.Show("There is already player in the map", "Object Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {
                                
                                Image = Image.FromFile("texture\\player.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                      
                            
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Player";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            objectList.Add(control);
                            RoteobjectList.Add(temp);
                            IsTherePlayer = true;
                        }
                        break;
                    case ControlNodeType.Wall:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\unattachwall.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Wall";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Width = 20;
                            control.Height = 20;
                            control.Tag = temp.ObjectID;


                          
                            RoteobjectList.Add(temp);
                            objectList.Add(control);
                 

                        }
                        break;
                    case ControlNodeType.AttachWall:
                    {
                        if (IsMakingMovingWall)
                        {
                            MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        }
                        control = new PictureBox()
                        {

                            Image = Image.FromFile("texture\\wall.png"),
                            SizeMode = PictureBoxSizeMode.StretchImage,

                        };
                        RoteObject temp = new RoteObject();
                        temp.objectstyle = "AttachWall";
                        temp.PositionX = control.Location.X;
                        temp.PositionY = control.Location.Y;
                        temp.ScaleX = control.Size.Width;
                        temp.ScaleY = control.Size.Height;
                        temp.ObjectID = RoteobjectList.Count;
                        control.Width = 20;
                        control.Height = 20;
                        control.Tag = temp.ObjectID;
                        RoteobjectList.Add(temp);
                        objectList.Add(control);


                    }
                        break;
                    case ControlNodeType.Button:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Button";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Width = 20;
                            control.Height = 20;
                            control.Tag = temp.ObjectID;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);
                  

                        }
                        break;
                    case ControlNodeType.Box:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\box.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Box";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);
                

                        }

                        break;
                    case ControlNodeType.AttachBox:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\AttachBox.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "AttachBox";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.limited3Trigger180_0:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\limited180_3.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };

                            
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger180";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.m_TriggerliftimeSet = 3;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);
                            

                        }
                        break;
                    case ControlNodeType.limited3Trigger180_90:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\limited180_3.png"),
                               
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image=
                                RotateImage(((System.Windows.Forms.PictureBox)control).Image,RotateFlipType.Rotate90FlipX);
                            
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger180";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 90;
                            temp.m_TriggerliftimeSet = 3;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);
                             

                        }
                        break;
                    case ControlNodeType.limited3Trigger180_180:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\limited180_3.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate180FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger180";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 180;
                            temp.m_TriggerliftimeSet = 3;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.limited3Trigger180_270:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\limited180_3.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
        RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate270FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger180";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.m_TriggerliftimeSet = 3;
                            temp.Rotation = 270;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.limited3Trigger90_0:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\limited90_3.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.m_TriggerliftimeSet = 3;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);
               

                        }
                        break;
                    case ControlNodeType.limited3Trigger90_90:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\limited90_3.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate90FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.m_TriggerliftimeSet = 3;
                            temp.Rotation = 90;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.limited3Trigger90_180:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\limited90_3.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate180FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.m_TriggerliftimeSet = 3;
                            temp.Rotation = 180;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.limited3Trigger90_270:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\limited90_3.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate270FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 270;
                            temp.m_TriggerliftimeSet = 3;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger180_0:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\180button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };


                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger180";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger180_90:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\180button.png"),

                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
                                RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate90FlipX);

                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger180";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 90;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger180_180:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\180button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate180FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger180";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 180;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger180_270:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\180button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
        RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate270FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger180";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 270;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger90_0:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\90button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger90_90:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\90button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate90FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 90;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger90_180:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\90button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate180FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 180;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger90_270:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\90button.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate270FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 270;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger90Right_0:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\90TriggerRight.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90Right";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger90Right_90:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\90TriggerRight.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate90FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90Right";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 90;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger90Right_180:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\90TriggerRight.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate180FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90Right";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 180;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger90Right_270:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\90TriggerRight.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            ((System.Windows.Forms.PictureBox)control).Image =
     RotateImage(((System.Windows.Forms.PictureBox)control).Image, RotateFlipType.Rotate270FlipX);
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Trigger90Right";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            temp.Rotation = 270;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Hazard:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\hazard.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Hazard";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);
                     
                        }
                        break;
                    case ControlNodeType.Camera:
                        {
                            if (IsMakingMovingWall)
                            {
                                MessageBox.Show("You Should Finish To Make MovingWall", "Spawning Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }

                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\camera.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Camera";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Tag = "Camera";
                            control.Width = 20;
                            control.Height = 20;
                            objectList.Add(control);

                            IsThereCamera = true;
                        }
                        break;
                    case ControlNodeType.Clearzone:
                        {
                            if (IsMakingMovingWall)
                                return;
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("texture\\clearzone.png"),
                                SizeMode = PictureBoxSizeMode.StretchImage,

                            };
                            RoteObject temp = new RoteObject();
                            temp.objectstyle = "Clearzone";
                            temp.PositionX = control.Location.X;
                            temp.PositionY = control.Location.Y;
                            temp.ScaleX = control.Size.Width;
                            temp.ScaleY = control.Size.Height;
                            temp.ObjectID = RoteobjectList.Count;
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);

                            IsThereClearZone = true;
                        }
                        break;
                    case ControlNodeType.MovingWall:
                    {
                        
                            control = new PictureBox()
                        {
                             Size= GetSizeOfLastMovingWall(),
                            Image = Image.FromFile("texture\\wall.png"),
                            SizeMode = PictureBoxSizeMode.StretchImage,

                        };
                        RoteObject temp = new RoteObject();
                            if(NumberOfPaths ==1)
                        temp.objectstyle = "MovingWall";
                        else if (NumberOfPaths > 1)
                        {
                            temp.objectstyle = "Path";
                             PathsList.Add(temp);

                        }

                        temp.PositionX = control.Location.X;
                        temp.PositionY = control.Location.Y;
                        temp.ScaleX = control.Size.Width;
                        temp.ScaleY = control.Size.Width;
                      
                            temp.ObjectID = RoteobjectList.Count;
                        
                           
                        temp.NumberOfPaths = NumberOfPaths;
                        
                            control.Tag = temp.ObjectID;
                 


                        
                                RoteobjectList.Add(temp);
                    
                         
                        objectList.Add(control);
                        IsMakingMovingWall = true;
                        
                            Brush blackBrush = new SolidBrush(Color.Red);                   
                        FontFamily familyName = new FontFamily("굴림");                   
                        System.Drawing.Font myFont = new System.Drawing.Font(familyName, 150, FontStyle.Regular, GraphicsUnit.Pixel);                                                        // 폰트생성
                        PointF startPoint = new PointF(control.Location.X+10, control.Location.Y+10);                           

                           Graphics g = Graphics.FromImage(((System.Windows.Forms.PictureBox)control).Image);
                      
                            g.DrawString(NumberOfMovingWalls.ToString() + " "+NumberOfPaths  , myFont, blackBrush, startPoint);
                        NumberOfPaths++;
                        TotalPaths++;
                    }
                
                        break;
                }


            }
            selectedNode.Tag = control;
  
           
           DoDragDrop(e.Item, DragDropEffects.Move);

        }

        private void button1_Click(object sender, EventArgs e)
        {

        }
        public int NumberChanged(int num)
        {
            bool IsMinus = (num <0)? true:false;
            string number = num.ToString();

            int Lastnumber = Int32.Parse(number[number.Length - 1].ToString());

            
            if(!IsMinus)
                return num - Lastnumber;
            else
                return num + Lastnumber;

        }

        internal void JsonTextUpdate()
        {

           // SynchronizationObjects();
            JArray Componentlist = new JArray();
            List<string> components = new List<string>();
            List<Point> Paths = new List<Point>();
            List<JProperty> PropertyList = new List<JProperty>();
            JObject tempObject = null;
            JObject camObject = null;
            JObject LevelInfo = null;
            string result = null;
            string objectinfo = null;
            string LevelInfoString = null;


            LevelInfo = new JObject(

                     new JProperty("Level", textBox1.Text),
                       new JProperty("NumberOfObjects", RoteobjectList.Count-PathsList.Count)
                     );

            LevelInfoString = LevelInfo.ToString();
            LevelInfoString = LevelInfoString.Remove(0, 1);
            LevelInfoString = LevelInfoString.Remove(LevelInfoString.Count() - 1, 1);
            LevelInfoString = LevelInfoString.Remove(LevelInfoString.Count() - 2, 1);
            LevelInfoString = LevelInfoString.Insert(LevelInfoString.Count() - 1, ",");
            result += LevelInfoString;
           int  indexOfPath = 0;



            foreach (var temp in RoteobjectList)
            {
                if (temp.objectstyle == "Path")
                    continue;

                if (temp.ObjectID == 1)
                {
                    string camString = null;
                    camObject = new JObject(

                        new JProperty("DefalutCamera",
                        new JObject(

                        new JProperty("EYE", new JObject(new JProperty("x", roCamera.PositionX), new JProperty("y", roCamera.PositionY), new JProperty("z", 999))))
                        )
                        );

                    camString = camObject.ToString();
                    camString = camString.Remove(0, 1);
                    camString = camString.Remove(camString.Count() - 1, 1);
                    camString = camString.Remove(camString.Count() - 2, 1);


                    camString = camString.Insert(camString.Count() - 1, ",");
                    result += camString;
                }




                if (temp.objectstyle == "Player")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("CONTROLLER");
                    components.Add("ANIMATION");
                    temp.Texture = "spritestrip.png";
                    temp.Mass = 1;
                    temp.Gravity = true;
                }

                if (temp.objectstyle == "AttachWall")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    temp.Texture = "wall.png";
                    temp.Mass = 0;
                }

                if (temp.objectstyle == "Button")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    temp.Texture = "1.png";
                    temp.Gravity = true;
                }

                if (temp.objectstyle == "Box")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    temp.Texture = "box.png";
                    temp.Gravity = false;
                    temp.Mass = 1;
                }
                if (temp.objectstyle == "AttachBox")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    temp.Texture = "AttachBox.png";
                    temp.Gravity = false;
                    temp.Mass = 1;
                }
                if (temp.objectstyle == "Trigger90")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("TRIGGER");
                    if (temp.m_TriggerliftimeSet == -1)
                        temp.Texture = "90button.png";
                    else
                        temp.Texture = "limited90_3.png";
                }

                if (temp.objectstyle == "Trigger180")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("TRIGGER");
                    if (temp.m_TriggerliftimeSet == -1)
                        temp.Texture = "180button.png";
                    else
                        temp.Texture = "limited180_3.png";
                }

                if (temp.objectstyle == "Clearzone")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    temp.Texture = "clearzone2.png";
                }

                if (temp.objectstyle == "Hazard")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    temp.Texture = "hazard.png";
                }
                if (temp.objectstyle == "Wall")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    temp.Texture = "unattachwall.png";
                    temp.Mass = 0;
                }
                if (temp.objectstyle == "MovingWall")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("AUTOMOVING");
                    temp.Texture = "wall.png";
                    foreach (var temp2 in RoteobjectList)
                    {

                        if (temp2.objectstyle == "Path")
                        {
                            Paths.Add(new Point((int)temp2.PositionX, (int)temp2.PositionY));
                            PropertyList.Add(new JProperty("Path" + indexOfPath,
                                new JObject(new JProperty("x", temp2.PositionX), new JProperty("y", temp2.PositionY))));
                            indexOfPath++;
                        }


                    }
                }
              

               




                foreach (var tempinComp in components)
                {
                    Componentlist.Add(tempinComp.ToString());
                }

     
                    
                tempObject = new JObject(

                     new JProperty("Object" + (temp.ObjectID+1 ).ToString(),
                         new JObject(
                    new JProperty("ObjectID", temp.ObjectID+1 ),
                      new JProperty("GravityOn", temp.Gravity),
                    new JProperty("Mass", temp.Mass),
                    new JProperty("Position", new JObject(new JProperty("x", temp.PositionX), new JProperty("y", temp.PositionY), new JProperty("z", temp.PositionZ))),
                    new JProperty("Rotation", temp.Rotation),
                    new JProperty("Scale", new JObject(new JProperty("x", temp.ScaleX), new JProperty("y", temp.ScaleY), new JProperty("z", temp.ScaleZ))),
                    new JProperty("TextureDir", temp.Texture),
                    new JProperty("DefaultTrigger180ornot", temp.Trigger180check),
                    new JProperty("Components", Componentlist),
                    new JProperty("TriggerLifeTime", temp.m_TriggerliftimeSet),
                    new JProperty("HavingComponentsNumbers", Componentlist.Count),
                    new JProperty("ObjectType", temp.objectstyle), PropertyList)
                    )
                    );
              
                objectinfo = tempObject.ToString();
                objectinfo = objectinfo.Remove(0, 1);
                objectinfo = objectinfo.Remove(objectinfo.Count() - 1, 1);
                objectinfo = objectinfo.Remove(objectinfo.Count() - 2, 1);
                Componentlist.Clear();
                components.Clear();
                if (temp.ObjectID + 1 != RoteobjectList.Count())
                    objectinfo = objectinfo.Insert(objectinfo.Count() - 1, ",");
                result += objectinfo;

            }
            result = result.Insert(0, "{");
            result = result.Insert(result.Count(), "}");

            textBox2.Text = result;

        }


        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void splitContainer1_SplitterMoved(object sender, SplitterEventArgs e)
        {
            
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            SynchronizationObjects();
        }

        private void propertyGrid1_Click(object sender, EventArgs e)
        {

        }

        private void propertyGrid1_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            
            

            RoteObject temp = (RoteObject)(propertyGrid1.SelectedObject);
            Point tempPoint = new Point((int)(temp.PositionX), (int)temp.PositionY);
            Control temp2 = null;

               for (int i = 0; i < this.designModePanel.Controls.Count; i++)
                {
                    if (this.designModePanel.Controls[i].Tag != null &&
                        (temp.ObjectID == Int32.Parse(this.designModePanel.Controls[i].Tag.ToString())))
                    {
                      this.designModePanel.Controls[i].Width = (int)temp.ScaleX;
                        this.designModePanel.Controls[i].Height = (int)temp.ScaleY;
                    this.designModePanel.Controls[i].Location = tempPoint;
                }

                }

           

        }

        private void button2_Click(object sender, EventArgs e)
        {
           
            
        }

        private void MainView_KeyDown(object sender, KeyEventArgs e)
        {
        
            RoteObject temp = (RoteObject)(propertyGrid1.SelectedObject);
            Control temp2 = null;
            int tempi = 3;
            if (e.KeyCode == Keys.Delete)
            {
                if (temp == null)
                    return;

                for(int i =0; i < this.designModePanel.Controls.Count; i++)
                {
                    if (temp.objectstyle == null)
                        return;
                     if( this.designModePanel.Controls[i].Tag != null &&
                         Int32.TryParse(this.designModePanel.Controls[i].Tag.ToString(), out tempi) &&
                        (temp.ObjectID == Int32.Parse(this.designModePanel.Controls[i].Tag.ToString())))
                    {
                        if (temp.objectstyle == "Player")
                            IsTherePlayer = false;
                        if (temp.objectstyle == "Camera")
                            break;
                        if (temp.objectstyle == "Clearzone")
                            IsThereClearZone = false;
                        RoteobjectList.Remove(temp);
                        this.designModePanel.Controls.Remove(this.designModePanel.Controls[i]);


                    }

                }

            }
        }

        private void MainView_KeyPress(object sender, KeyPressEventArgs e)
        {
           
        }

        private void chkDesignMode_CheckedChanged_1(object sender, EventArgs e)
        {

        }

        private void saveFileDialog2_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void Loadbutton_Click(object sender, EventArgs e)
        {
          

       
    


        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {

        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SynchronizationObjects();
            string file_path = null;
            string file = null;

            if (!InstantSavingCheck.Checked)
            {
                saveFileDialog1.InitialDirectory = Application.StartupPath;

                if (saveFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    file_path = saveFileDialog1.FileName;
                    file = file_path.Split('\\')[file_path.Split('\\').Length - 1];
                }
                else
                    return;

            }
            JArray Componentlist = new JArray();
            List<string> components = new List<string>();
            List<JProperty> PropertyList = new List<JProperty>();
            List<Point> Paths = new List<Point>();
            JObject tempObject = null;
            JObject camObject = null;
            JObject LevelInfo = null;
            string result = null;
            string objectinfo = null;
            string LevelInfoString = null;
            int indexOfPath = 0;

            LevelInfo = new JObject(

                     new JProperty("Level", textBox1.Text),
                       new JProperty("NumberOfObjects", RoteobjectList.Count)
                     );

            LevelInfoString = LevelInfo.ToString();
            LevelInfoString = LevelInfoString.Remove(0, 1);
            LevelInfoString = LevelInfoString.Remove(LevelInfoString.Count() - 1, 1);
            LevelInfoString = LevelInfoString.Remove(LevelInfoString.Count() - 2, 1);
            LevelInfoString = LevelInfoString.Insert(LevelInfoString.Count() - 1, ",");
            result += LevelInfoString;
            foreach (var temp in RoteobjectList)
            {

                if (temp.ObjectID == 0)
                {
                    string camString = null;
                    camObject = new JObject(

                        new JProperty("DefalutCamera",
                        new JObject(

                        new JProperty("EYE", new JObject(new JProperty("x", roCamera.PositionX), new JProperty("y", roCamera.PositionY), new JProperty("z", 999))))
                        )
                        );

                    camString = camObject.ToString();
                    camString = camString.Remove(0, 1);
                    camString = camString.Remove(camString.Count() - 1, 1);
                    camString = camString.Remove(camString.Count() - 2, 1);

                    camString = camString.Insert(camString.Count() - 1, ",");
                    result += camString;
                }



                if (temp.objectstyle == "Player")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("CONTROLLER");
                    components.Add("ANIMATION");
                    temp.Texture = "player.png";
                    temp.Mass = 1;
                    temp.Gravity = true;
                }

                if (temp.objectstyle == "AttachWall")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    temp.Texture = "wall.png";
                    temp.Mass = 0;
                }
                if (temp.objectstyle == "Wall")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    temp.Texture = "unattachwall.png";
                    temp.Mass = 0;
                }
                if (temp.objectstyle == "Button")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    temp.Texture = "1.png";
                    temp.Gravity = true;
                }

                if (temp.objectstyle == "Box")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    temp.Texture = "box.png";
                    temp.Gravity = false;
                    temp.Mass = 1;
                }

                if (temp.objectstyle == "Trigger90")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("TRIGGER");
                    if (temp.m_TriggerliftimeSet == -1)
                        temp.Texture = "90button.png";
                    else
                        temp.Texture = "limited90_3.png";




                }

                if (temp.objectstyle == "Trigger90Right")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("TRIGGER");
                    if (temp.m_TriggerliftimeSet == -1)
                        temp.Texture = "90TriggerRight.png";
                    else
                        temp.Texture = "90TriggerRight.png";




                }

                if (temp.objectstyle == "Trigger180")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("TRIGGER");
                    if (temp.m_TriggerliftimeSet == -1)
                        temp.Texture = "180button.png";
                    else
                        temp.Texture = "limited180_3.png";

                }

                if (temp.objectstyle == "Clearzone")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    temp.Texture = "clearzone.png";
                }

                if (temp.objectstyle == "Hazard")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    temp.Texture = "hazard.png";
                }

                if (temp.objectstyle == "MovingWall")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("AUTOMOVING");
                    temp.Texture = "wall.png";
                    foreach (var temp2 in RoteobjectList)
                    {

                        if (temp2.objectstyle == "Path")
                        {
                            Paths.Add(new Point((int)temp2.PositionX, (int)temp2.PositionY));
                            PropertyList.Add(new JProperty("Path" + indexOfPath,
                                new JObject(new JProperty("x", temp2.PositionX), new JProperty("y", temp2.PositionY))));
                            indexOfPath++;
                        }


                    }
                }





                foreach (var tempinComp in components)
                {
                    Componentlist.Add(tempinComp.ToString());
                }
                temp.PositionX = (temp.PositionX - (designModePanel.Size.Width / 2)) + (temp.ScaleX / 2);
                temp.PositionY = -(temp.PositionY - ((designModePanel.Size.Height / 2))) - (temp.ScaleY / 2);

                tempObject = new JObject(

                     new JProperty("Object" + (temp.ObjectID + 1).ToString(),
                         new JObject(
                    new JProperty("ObjectID", temp.ObjectID + 1),
                      new JProperty("GravityOn", temp.Gravity),
                    new JProperty("Mass", temp.Mass),
                    new JProperty("Position", new JObject(new JProperty("x", temp.PositionX), new JProperty("y", temp.PositionY), new JProperty("z", temp.PositionZ))),
                    new JProperty("Rotation", temp.Rotation),
                    new JProperty("Scale", new JObject(new JProperty("x", temp.ScaleX), new JProperty("y", temp.ScaleY), new JProperty("z", temp.ScaleZ))),
                    new JProperty("TextureDir", temp.Texture),
                    new JProperty("DefaultTrigger180ornot", temp.Trigger180check),
                    new JProperty("TriggerLifeTime", temp.m_TriggerliftimeSet),
                    new JProperty("Components", Componentlist),
                    new JProperty("HavingComponentsNumbers", Componentlist.Count),
                    new JProperty("ObjectType", temp.objectstyle))
                    )
                    );
                objectinfo = tempObject.ToString();
                objectinfo = objectinfo.Remove(0, 1);
                objectinfo = objectinfo.Remove(objectinfo.Count() - 1, 1);
                objectinfo = objectinfo.Remove(objectinfo.Count() - 2, 1);
                Componentlist.Clear();
                components.Clear();
                if (temp.ObjectID + 1 != RoteobjectList.Count())
                    objectinfo = objectinfo.Insert(objectinfo.Count() - 1, ",");
                result += objectinfo;

            }






            result = result.Insert(0, "{");
            result = result.Insert(result.Count(), "}");
            if (!IsThereCamera)
                MessageBox.Show("There is no Camera", "Object Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            if (!IsThereClearZone)
                MessageBox.Show("There is no ClearZone", "Object Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            if (!IsTherePlayer)
                MessageBox.Show("There is no Player", "Object Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            if (!IsTherePlayer || !IsThereClearZone || !IsThereCamera)
                return;
            if (InstantSavingCheck.Checked)
                File.WriteAllText("levels.\\" + textBox1.Text + ".json", result);
            else
                File.WriteAllText(file_path + ".json", result);
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RoteobjectList.Clear();

            objectList.Clear();

            for (int i = 0, objectcount = this.designModePanel.Controls.Count - 2; i < objectcount; i++)
            {

                this.designModePanel.Controls.RemoveAt(this.designModePanel.Controls.Count - 1);
            }
            string file_path = null;
            string file = null;

            if (!InstantSavingCheck.Checked)
            {
                openFileDialog1.InitialDirectory = Application.StartupPath;

                if (openFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    file_path = openFileDialog1.FileName;
                    file = file_path.Split('\\')[file_path.Split('\\').Length - 1];
                }
                else
                {
                    return;
                }

            }

            JObject Reading = null;
            if (InstantSavingCheck.Checked)
                Reading = JObject.Parse(File.ReadAllText("levels.\\" + textBox1.Text + ".json"));
            else
                Reading = JObject.Parse(File.ReadAllText(file_path));

            int iNumberOfObjects = Int32.Parse(Reading["NumberOfObjects"].ToString());

            for (int i = 0; i < iNumberOfObjects; i++)
            {
                RoteObject temp = new RoteObject();
                string ObjectNumberString = "Object" + (i + 1).ToString();

                temp.PositionX = System.Single.Parse(Reading[ObjectNumberString]["Position"]["x"].ToString());
                temp.PositionY = System.Single.Parse(Reading[ObjectNumberString]["Position"]["y"].ToString());

                temp.PositionZ = System.Single.Parse(Reading[ObjectNumberString]["Position"]["z"].ToString());
                temp.ScaleX = System.Single.Parse(Reading[ObjectNumberString]["Scale"]["x"].ToString());
                temp.ScaleY = System.Single.Parse(Reading[ObjectNumberString]["Scale"]["y"].ToString());


                ///////////////////////////////////Convert Position
                temp.PositionX = ((temp.PositionX + (designModePanel.Size.Width / 2)) - (temp.ScaleX / 2));
                temp.PositionY = (-temp.PositionY + ((designModePanel.Size.Height / 2)));
                /// ///////////////////////////////
                temp.Rotation = System.Single.Parse(Reading[ObjectNumberString]["Rotation"].ToString());
                temp.ObjectID = i;
                temp.objectstyle = Reading[ObjectNumberString]["ObjectType"].ToString();
                if (temp.objectstyle == "Player")
                    IsTherePlayer = true;
                if (temp.objectstyle == "Clearzone")
                    IsThereClearZone = true;
                IsThereCamera = true;
                 
                temp.Texture = Reading[ObjectNumberString]["TextureDir"].ToString();

                RoteobjectList.Add(temp);
            }
            Control tempCamera = new PictureBox();
            Point tempCameraLocation = new Point(Int32.Parse(Reading["DefalutCamera"]["EYE"]["x"].ToString())
            , Int32.Parse(Reading["DefalutCamera"]["EYE"]["y"].ToString()));
            tempCamera.Location = tempCameraLocation;
            tempCamera.Size = new Size(50, 50);
            ((System.Windows.Forms.PictureBox)tempCamera).Image = Image.FromFile("texture\\" + "camera.png");
            ((System.Windows.Forms.PictureBox)tempCamera).SizeMode = PictureBoxSizeMode.StretchImage;
            tempCamera.Tag = 0;
            this.designModePanel.Controls.Add(tempCamera);
            objectList.Add(tempCamera);

            for (int i = 0; i < iNumberOfObjects; i++)
            {
                Control tempC = new PictureBox();
                Point tempLocation = new Point((int)RoteobjectList[i].PositionX, (int)RoteobjectList[i].PositionY);
                Size tempScale = new Size((int)RoteobjectList[i].ScaleX, (int)RoteobjectList[i].ScaleY);
                tempC.Tag = RoteobjectList[i].ObjectID;
                if (RoteobjectList[i].Texture == "0")
                    ((System.Windows.Forms.PictureBox)tempC).Image = Image.FromFile("texture\\" + "wall.png");
                else
                    ((System.Windows.Forms.PictureBox)tempC).Image = Image.FromFile("texture\\" + RoteobjectList[i].Texture);
                ((System.Windows.Forms.PictureBox)tempC).SizeMode = PictureBoxSizeMode.StretchImage;
                tempC.Location = tempLocation;
                tempC.Size = tempScale;

                this.designModePanel.Controls.Add(tempC);
                objectList.Add(tempC);

            }

        }

        private void designModePanel_Paint(object sender, PaintEventArgs e)
        {
         
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void treeControls_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }
    }

 

}
