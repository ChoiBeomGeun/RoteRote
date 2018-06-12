/******************************************************************************/
/*!
\file   Object.cs
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    RoteRoteMapEditor
\date   2018/6/6

Object Interface of Map Editor
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.ComponentModel;
namespace RoteRoteLauncherView
{

    struct vector3
    {
        float x;
        float y;
        float z;


    }

    public class RoteObject
    {
   
        private string m_strName;
        private float m_posX;
        private float m_posY;
        private float m_posZ;
        private float m_scaleX;
        private float m_scaleY;
        private float m_scaleZ;

        private bool m_defaultGravity;
        private int m_defaultMass;

        private string m_Texture;
        private int m_rotation;

        private bool m_Trigger180ornot;
        public RoteObject()
        {

        }
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
        public int Rotation
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

    }
}
