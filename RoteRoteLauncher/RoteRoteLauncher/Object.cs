﻿using System;
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
        private int m_posX;
        private int m_posY;
        private int m_posZ;
        private int m_scaleX;
        private int m_scaleY;
        private int m_scaleZ;

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
        public int PositionX
        {
            set { m_posX = value; }
            get { return m_posX; }
        }
        [Category("Transform")]
        [DisplayName("PositionY")]
        public int PositionY
        {
            set { m_posY = value; }
            get { return m_posY; }
        }
        [Category("Transform")]
        [DisplayName("PositionZ")]
        public int PositionZ
        {
            set { m_posZ = value; }
            get { return m_posZ; }
        }
        #endregion 
        #region Scale 
        [Category("Transform")]
        [DisplayName("ScaleX")]
        public int ScaleX
        {
            set { m_scaleX = value; }
            get { return m_scaleX; }
        }
        [Category("Transform")]
        [DisplayName("ScaleY")]
        public int ScaleY
        {
            set { m_scaleY = value; }
            get { return m_scaleY; }
        }
        [Category("Transform")]
        [DisplayName("ScaleZ")]
        public int ScaleZ
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