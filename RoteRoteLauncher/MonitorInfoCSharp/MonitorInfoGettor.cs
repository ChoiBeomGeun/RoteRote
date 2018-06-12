/******************************************************************************/
/*!
\file   MonitorInfoGettor.cs
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    RoteRoteMapEditor
\date   2018/6/6

Getting max size of monitor
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

using System; 
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Management;

namespace MonitorInfoCSharp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            Size newsize = returnMaxSize();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ManagementObjectSearcher mosDesktop = new ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_DesktopMonitor");

            foreach (ManagementObject moDesktop in mosDesktop.Get())
            {
                if (moDesktop["Description"] != null)
                    MessageBox.Show( moDesktop["Description"].ToString());

            }
        }


        static public Size returnMaxSize()
        {
            var scope = new System.Management.ManagementScope();
            var q = new System.Management.ObjectQuery("SELECT * FROM CIM_VideoControllerResolution");
            UInt32 maxHResolution = 0;
            UInt32 maxVResolution = 0;
            using (var searcher = new System.Management.ManagementObjectSearcher(scope, q))
            {
                var results = searcher.Get();
        

                foreach (var item in results)
                {
                    if ((UInt32)item["HorizontalResolution"] > maxHResolution)
                        maxHResolution = (UInt32)item["HorizontalResolution"];

                    if ((UInt32)item["VerticalResolution"] > maxVResolution)
                        maxVResolution = (UInt32)item["VerticalResolution"];
                }

                // log.Debug("Max Supported Resolution " + maxHResolution + "x" + maxVResolution);
            }
            return new Size((int)maxHResolution, (int)maxVResolution);



        }
    }


}
