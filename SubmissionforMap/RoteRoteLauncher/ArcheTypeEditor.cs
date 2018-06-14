using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp6
{


     enum ComponentType {


        CT_INVALID = 0,
        CT_TRANSFORM,
        CT_CONTROLLER,
        CT_SPRITE,
        /*CT_PARTICLE,*/
        CT_BODY,
        CT_BUTTON,
        CT_TRIGGER,
        CT_ANIMATION,
        CT_AutoMoving,
        CT_NUMCOMPONENT


    };


    enum Objectstyle
    {
        Player, Asteriod, Wall, Button, Box, Trigger90, Trigger180, Clearzone, Hazard, Camera
    };

    public struct Position
    {
        int x;
        int y;
    };

    public struct star
    {
  
        string name;
        int y;
    };

    public  class RoteObj
    {
        Position pos;
        star mstar;
      
         
    };
    public partial class ArcheTypeeditor : Form
    {
        RoteObj temp = new RoteObj();
        List<RoteObj> Objectlist = new List<RoteObj>();
        List<ComponentType> ComponentLists = new List<ComponentType>();
        public ArcheTypeeditor()
        {
            RoteObj temp = new RoteObj() ;
            Objectlist.Add(temp);
            
            InitializeComponent();
            ComponentBox.DataSource = Enum.GetNames(typeof(ComponentType));
            ObjectstyleCombo.DataSource = Enum.GetNames(typeof(Objectstyle));
            propertyGrid1.SelectedObject = temp;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void NameLabel_Click(object sender, EventArgs e)
        {

        }

        private void ObjectstyleCombo_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void ArcheTypeEditor_Load(object sender, EventArgs e)
        {

        }
    }
}
