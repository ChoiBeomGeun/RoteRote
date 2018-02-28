using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json;
using System.IO;
namespace RoteRoteLauncherView
{
    enum ComponentType
    {


        INVALID = 0,
        TRANSFORM,
        CONTROLLER,
        SPRITE,
        PARTICLE,
        BODY,
        BUTTON,
        TRIGGER,
        ANIMATION,
        AUTOMOVING,
        NUMCOMPONENT


    };


    enum Objectstyle
    {
        Player, Asteriod, Wall, Button, Box, Trigger90, Trigger180, Clearzone, Hazard, Camera
    };
    public partial class ArcheTypeEditorForm : Form
    {
        RoteObject prop = new RoteObject();
        List<string> ComponentList = new List<string>();
        public ArcheTypeEditorForm()
        {
            InitializeComponent();
        }

      

        private void Form1_Load(object sender, EventArgs e)
        {
        

   
         
          
            propertyGrid1.SelectedObject = prop;
            ComponentBox.DataSource = Enum.GetNames(typeof(ComponentType));
            ObjectStyleBox.DataSource = Enum.GetNames(typeof(Objectstyle));

        }

        private void propertyGrid1_Click(object sender, EventArgs e)
        {

        }

        private void ComponentBox_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void Add_Click(object sender, EventArgs e)
        {
           foreach(string listBox in listBox1.Items )
            {
                if (listBox == ComponentBox.SelectedItem.ToString())
                    return;
            }

            listBox1.Items.Add(ComponentBox.SelectedItem);
        
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void Savebutton1_Click(object sender, EventArgs e)
        {
            string file_path = null;
            string file = null;
            saveFileDialog1.InitialDirectory = Application.StartupPath;

            if(saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                file_path = saveFileDialog1.FileName;
                file = file_path.Split('\\')[file_path.Split('\\').Length - 1];
            }
            

            JArray Componentlist = new JArray();
           foreach(var temp in listBox1.Items)
            {
                Componentlist.Add(temp.ToString());
            }
            JObject tempObject = new JObject(
                new JProperty("ArchtypeName", ArcheTypeTextBox.Text),
                new JProperty("DefaultGravity", prop.Gravity),
                new JProperty("DefaultMass", prop.Mass),
                new JProperty("DefaultPosition", new JObject(new JProperty("x", prop.PositionX), new JProperty("y", prop.PositionX), new JProperty("z", prop.PositionX))),
                new JProperty("DefaultRotation", prop.Rotation),
                new JProperty("DefaultScale", new JObject(new JProperty("x", prop.ScaleX), new JProperty("y", prop.ScaleY), new JProperty("z", prop.ScaleZ))),
                new JProperty("DefaultTexture", prop.Texture),
                new JProperty("DefaultTrigger180ornot", prop.Trigger180check),
                new JProperty("Components", Componentlist),
                new JProperty("HavingComponentsNumbers", Componentlist.Count),
                new JProperty("ObjectStyle", ObjectStyleBox.SelectedItem.ToString())

                );

            File.WriteAllText(file_path+".json", tempObject.ToString());
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void saveFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }
    }
}
