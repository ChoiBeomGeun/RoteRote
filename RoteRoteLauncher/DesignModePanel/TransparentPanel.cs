using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ControlDesignMode
{
    /// <summary>
    /// 배경이 투명한 보더 패널
    /// </summary>
    internal class TransparentPanel : Panel
    {
        internal TransparentPanel()
        {
            // don't paint the background
            SetStyle(ControlStyles.Opaque, true);
        }

        protected override CreateParams CreateParams
        {
            get
            {
                // use transparency
                CreateParams cp = base.CreateParams;
                cp.ExStyle |= 0x00000020; //WS_EX_TRANSPARENT
                return cp;
            }
        }
    }

}
