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
namespace RoteMapView
{



    public partial class MainView : Form
    {
       public bool IsThereClearZone = false;
       public bool IsThereCamera = false;
       public bool IsTherePlayer = false;
        List<Control> objectList = new List<Control>();
        List<RoteObject> RoteobjectList = new List<RoteObject>();
        RoteObject roCamera = new RoteObject();

        enum ControlNodeType
        {
            Player, Wall, Button, Box,
            Trigger90_0, Trigger90_90, Trigger90_180,Trigger90_270,
            Trigger180_0, Trigger180_90, Trigger180_180,Trigger180_270,
            Clearzone, Hazard, Camera
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
                if (RoteobjectList.Count-1 < Int32.Parse(args.Control.Tag.ToString()))
                    return;
                RoteobjectList[Int32.Parse(args.Control.Tag.ToString())].PositionX = args.Control.Location.X;
                RoteobjectList[Int32.Parse(args.Control.Tag.ToString())].PositionY = args.Control.Location.Y;
                RoteobjectList[Int32.Parse(args.Control.Tag.ToString())].ScaleX = args.Control.Size.Width;
                RoteobjectList[Int32.Parse(args.Control.Tag.ToString())].ScaleY = args.Control.Size.Height;
               
                propertyGrid1.SelectedObject = RoteobjectList[Int32.Parse(args.Control.Tag.ToString())];

               
               

            }

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
                            if (IsTherePlayer)
                            {
                                MessageBox.Show("There is already player in the map", "Object Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                return;
                            }
                            control = new PictureBox()
                            {
                                
                                Image = Image.FromFile("./player.png"),
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
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./wall.png"),
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
                    case ControlNodeType.Button:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./button.png"),
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
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./box.png"),
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
                    case ControlNodeType.Trigger180_0:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./180button.png"),
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
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./180button.png"),
                               
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
                            control.Tag = temp.ObjectID;
                            control.Width = 20;
                            control.Height = 20;
                            RoteobjectList.Add(temp);
                            objectList.Add(control);


                        }
                        break;
                    case ControlNodeType.Trigger180_180:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./180button.png"),
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
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./180button.png"),
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
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./90button.png"),
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
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./90button.png"),
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
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./90button.png"),
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
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./90button.png"),
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
                    case ControlNodeType.Hazard:
                        {
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./hazard.png"),
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
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./camera.png"),
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
                            control = new PictureBox()
                            {

                                Image = Image.FromFile("./clearzone.png"),
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
                }


            }
            selectedNode.Tag = control;
  

            DoDragDrop(e.Item, DragDropEffects.Move);

        }

        private void button1_Click(object sender, EventArgs e)
        {
            SynchronizationObjects();
            string file_path = null;
            string file = null;
            saveFileDialog1.InitialDirectory = Application.StartupPath;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                file_path = saveFileDialog1.FileName;
                file = file_path.Split('\\')[file_path.Split('\\').Length - 1];
            }


            JArray Componentlist = new JArray();
            List<string> components= new List<string>();

            JObject tempObject= null;
            JObject camObject = null;
            JObject LevelInfo = null;
            string result = null;
            string objectinfo = null;
            string LevelInfoString = null;


            LevelInfo = new JObject(

                     new JProperty("Level" ,textBox1.Text),
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

                if (temp.ObjectID ==1)
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

                if (temp.objectstyle == "Wall")
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

                if (temp.objectstyle == "Trigger90")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("TRIGGER");
                    temp.Texture = "90button.png";
                }

                if (temp.objectstyle == "Trigger180")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("TRIGGER");
                    temp.Texture = "180button.png";
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



               
                


                foreach (var tempinComp in components)
                {
                    Componentlist.Add(tempinComp.ToString());
                }
                temp.PositionX = (temp.PositionX - (designModePanel.Size.Width / 2)) + (temp.ScaleX/2);
                temp.PositionY = -(temp.PositionY - ((designModePanel.Size.Height / 2))) - (temp.ScaleY / 2);
  
                tempObject = new JObject(

                     new JProperty("Object"+(temp.ObjectID+1).ToString(),
                         new JObject(
                    new JProperty("ObjectID", temp.ObjectID + 1),
                      new JProperty("GravityOn", temp.Gravity),
                    new JProperty("Mass", temp.Mass),
                    new JProperty("Position", new JObject(new JProperty("x", temp.PositionX), new JProperty("y", temp.PositionY), new JProperty("z", temp.PositionZ))),
                    new JProperty("Rotation", temp.Rotation),
                    new JProperty("Scale", new JObject(new JProperty("x", temp.ScaleX), new JProperty("y", temp.ScaleY), new JProperty("z", temp.ScaleZ))),
                    new JProperty("TextureDir", temp.Texture),
                    new JProperty("DefaultTrigger180ornot", temp.Trigger180check),
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
                if (temp.ObjectID+1 != RoteobjectList.Count())
                objectinfo = objectinfo.Insert(objectinfo.Count() - 1, ",");
                result += objectinfo;
                
            }
         





            result= result.Insert(0, "{");
            result= result.Insert(result.Count(), "}");
            if(!IsThereCamera)
                MessageBox.Show("There is no Camera", "Object Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            if (!IsThereClearZone)
                MessageBox.Show("There is no ClearZone", "Object Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            if (!IsTherePlayer)
                MessageBox.Show("There is no Player", "Object Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            if (!IsTherePlayer || !IsThereClearZone || !IsThereCamera)
                return;

            File.WriteAllText(file_path + ".json", result);
        }


         internal void JsonTextUpdate()
        {

           // SynchronizationObjects();
            JArray Componentlist = new JArray();
            List<string> components = new List<string>();

            JObject tempObject = null;
            JObject camObject = null;
            JObject LevelInfo = null;
            string result = null;
            string objectinfo = null;
            string LevelInfoString = null;


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

                if (temp.objectstyle == "Wall")
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

                if (temp.objectstyle == "Trigger90")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("TRIGGER");
                    temp.Texture = "90button.png";
                }

                if (temp.objectstyle == "Trigger180")
                {
                    components.Add("TRANSFORM");
                    components.Add("SPRITE");
                    components.Add("BODY");
                    components.Add("TRIGGER");
                    temp.Texture = "180button.png";
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


            
                


                foreach (var tempinComp in components)
                {
                    Componentlist.Add(tempinComp.ToString());
                }
          

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
            Point tempPoint = new Point(temp.PositionX, temp.PositionY);
            Control temp2 = null;

               for (int i = 0; i < this.designModePanel.Controls.Count; i++)
                {
                    if (this.designModePanel.Controls[i].Tag != null &&
                        (temp.ObjectID == Int32.Parse(this.designModePanel.Controls[i].Tag.ToString())))
                    {
                      this.designModePanel.Controls[i].Width = temp.ScaleX;
                        this.designModePanel.Controls[i].Height = temp.ScaleY;
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

            if (e.KeyCode == Keys.Delete)
            {
                if (temp == null)
                    return;

                for(int i =0; i < this.designModePanel.Controls.Count; i++)
                {
                    if (this.designModePanel.Controls[i].Tag != null &&
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
    }
}
