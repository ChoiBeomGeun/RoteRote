/******************************************************************************/
/*!
\file   TransparentPanel.cs
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    RoteRoteMapEditor
\date   2018/6/6

Transparent Panel Interface
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ControlDesignMode
{

    internal class TransparentPanel : Panel
    {
        internal TransparentPanel()
        {
      
            SetStyle(ControlStyles.Opaque, true);
        }

        protected override CreateParams CreateParams
        {
            get
            {
             
                CreateParams cp = base.CreateParams;
                cp.ExStyle |= 0x00000020;
                return cp;
            }
        }
    }

}
