/******************************************************************************/
/*!
\file   Program.cs
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    RoteRoteMapEditor
\date   2018/6/6

Main Entry of RoteMapEditorView
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace RoteMapView
{
    static class Program
    { 

        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainView());
        }
    }
}
