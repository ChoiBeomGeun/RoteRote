using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.ComponentModel;
namespace RoteMapView
{

    struct vector3
    {
        float x;
        float y;
        float z;


    }

    public class RoteObject
    {
        private int m_ojbectid;
        private int m_NumberOfPaths;
        private string m_strName;
        private string m_objectstyle;
        private float m_posX;
        private float m_posY;
        private float m_posZ;
        private float m_scaleX;
        private float m_scaleY;
        private float m_scaleZ;

        private bool m_defaultGravity;
        private int m_defaultMass;

        private string m_Texture;
        private float m_rotation;

        private bool m_Trigger180ornot;
        public RoteObject()
        {

        }
        #region ObjectInfomation
        [Category("ObjectInfomation")]
        [DisplayName("ObjectID")]
        public int ObjectID
        {
            set { m_ojbectid = value; }
            get { return m_ojbectid; }
        }

        [Category("ObjectInfomation")]
        [DisplayName("ObjecStyle")]
        public string objectstyle
        {
            set { m_objectstyle = value; }
            get { return m_objectstyle; }
        }
        #endregion
        #region Position
        [Category("Transform")]
        [DisplayName("PositionX")]
        public float PositionX
        {
            set { m_posX = value; }
            get { return m_posX; }
        }
        [Category("Transform")]
        [DisplayName("PositionY")]
        public float PositionY
        {
            set { m_posY = value; }
            get { return m_posY; }
        }
        [Category("Transform")]
        [DisplayName("PositionZ")]
        public float PositionZ
        {
            set { m_posZ = value; }
            get { return m_posZ; }
        }
        #endregion 
        #region Scale 
        [Category("Transform")]
        [DisplayName("ScaleX")]
        public float ScaleX
        {
            set { m_scaleX = value; }
            get { return m_scaleX; }
        }
        [Category("Transform")]
        [DisplayName("ScaleY")]
        public float ScaleY
        {
            set { m_scaleY = value; }
            get { return m_scaleY; }
        }
        [Category("Transform")]
        [DisplayName("ScaleZ")]
        public float ScaleZ
        {
            set { m_scaleZ = value; }
            get { return m_scaleZ; }
        }
        #endregion
        #region Body
        [Category("Body")]
        [DisplayName("Mass")]
        public int Mass
        {
            set { m_defaultMass = value; }
            get { return m_defaultMass; }
        }


        [Category("Body")]
        [DisplayName("IsGravityOn")]
        public bool Gravity
        {
            set { m_defaultGravity = value; }
            get { return m_defaultGravity; }
        }
        #endregion
        #region Sprite
        [Category("Sprite")]
        [DisplayName("Texture")]
        public string Texture
        {
            set { m_Texture = value; }
            get { return m_Texture; }
        }

        [Category("Sprite")]
        [DisplayName("Rotation")]
        public float Rotation
        {
            set { m_rotation = value; }
            get { return m_rotation; }
        }
        #endregion
        #region Trigger
        [Category("Trigger")]
        [DisplayName("Trigger180ornot")]
        public bool Trigger180check
        {
            set { m_Trigger180ornot = value; }
            get { return m_Trigger180ornot; }
        }
        #endregion
        #region MovingPaths
        [Category("MovingPaths")]
        [DisplayName("NumberOfPaths")]
        public int NumberOfPaths
        {
            set { m_NumberOfPaths = value; }
            get { return m_NumberOfPaths; }
        }
        #endregion
    }
}
