/******************************************************************************/
/*!
\file   Program.cs
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    RoteRoteMapEditor
\date   2018/6/6

Main Entry of MonitorInfoGettor
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace MonitorInfoCSharp
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
