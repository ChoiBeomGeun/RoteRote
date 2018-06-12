/******************************************************************************/
/*!
\file   ControlDesignModePanel.cs
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    RoteRoteMapEditor
\date   2018/6/6

Control Design Panel Implementation
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

    public class ControlDesignModePanel : Panel
    {
     
        bool isControlMoving = false;

        TransparentPanel transparentPanel;

        SelectedControlViewport selectedControlViewport = new SelectedControlViewport();

        Point childControlClickedPoint;

        bool isControlChanging = false;

        bool isDesignMode = true;

        public bool IsDesignMode
        {
            get { return isDesignMode; }
            set 
            { 
                isDesignMode = value;
                if (isDesignMode == false)
                {
                    this.SelectedControl = null;
                    this.RemoveBorderPanel();
                    this.Cursor = Cursors.Default;
                }
                else
                {
                    this.AddBorderPanel();
                }
            }
        }

        Control selectedControl = null;

        internal Control SelectedControl
        {
            get { return selectedControl; }
            set
            {
                if (value == transparentPanel)
                    return;

                if (this.selectedControl != null)
                {
                    Rectangle rect = GetSelectedObjectRect();
                    this.selectedControl = null;
                    this.Invalidate(rect);
                }
                this.selectedControl = value;
                InvalidateSelectedObject();
            }
        }
 
        public event MouseDownEventHandler MouseRightClick;
        public event MouseDownEventHandler MouseLeftClick;

        public event PaintEventHandler Painted;
       
        public ControlDesignModePanel()
        {
            this.DoubleBuffered = true;
            this.AllowDrop = true;

            this.DragOver += DrawPanel_DragOver;
            this.DragDrop += DrawPanel_DragDrop;

            this.ControlAdded += DrawPanel_ControlAdded;
            this.ControlRemoved += DrawPanel_ControlRemoved;

            this.MouseMove += DrawPanel_MouseMove;
            this.MouseDown += DrawPanel_MouseDown;
            this.MouseUp += DrawPanel_MouseUp;

            this.Paint += OnPaint;
        }


        private void AddBorderPanel()
        {
            this.RemoveBorderPanel();

            this.transparentPanel = new TransparentPanel();

            this.transparentPanel.Dock = DockStyle.Fill;
            this.transparentPanel.MouseMove += TransparentPanel_MouseMove;
            this.transparentPanel.MouseDown += TransparentPanel_MouseDown;
            this.transparentPanel.MouseUp += TransparentPanel_MouseUp;

            this.Controls.Add(transparentPanel);

            transparentPanel.BringToFront();
        }

     
        private void RemoveBorderPanel()
        {
            if (transparentPanel != null)
            {
                this.transparentPanel.MouseMove -= TransparentPanel_MouseMove;
                this.transparentPanel.MouseDown -= TransparentPanel_MouseDown;
                this.transparentPanel.MouseUp -= TransparentPanel_MouseUp;

                this.Controls.Remove(transparentPanel);
                this.transparentPanel.Dispose();
            }
        }

        private bool IsDrawSelectedControlBorder()
        {
            return isDesignMode && this.selectedControl != null && this.isControlChanging == false;
        }


        private void TransparentPanel_MouseUp(object sender, MouseEventArgs e)
        {
            Point thisCtlPoint = this.PointToClient(MousePosition);
            DrawPanel_MouseUp(sender, new MouseEventArgs(e.Button, e.Clicks, thisCtlPoint.X, thisCtlPoint.Y, e.Delta));
        }


        private void TransparentPanel_MouseDown(object sender, MouseEventArgs e)
        {
            Point thisCtlPoint = this.PointToClient(MousePosition);
            DrawPanel_MouseDown(sender, new MouseEventArgs(e.Button, e.Clicks, thisCtlPoint.X, thisCtlPoint.Y, e.Delta));
        }


        private void TransparentPanel_MouseMove(object sender, MouseEventArgs e)
        {
            Point thisCtlPoint = this.PointToClient(MousePosition);
            DrawPanel_MouseMove(sender, new MouseEventArgs(e.Button, e.Clicks, thisCtlPoint.X, thisCtlPoint.Y ,e.Delta));
        }


        private Rectangle GetSelectedObjectRect()
        {
            Rectangle rect = this.selectedControl.Bounds;
            rect.Inflate(SelectedControlViewport.DRAG_HANDLE_SIZE, SelectedControlViewport.DRAG_HANDLE_SIZE);
            return rect;
        }


        private void InvalidateSelectedObject()
        {
            if (this.selectedControl != null)
            {
                this.Invalidate(GetSelectedObjectRect(), true);
            }
        }
        public int NumberChanged(int num)
        {
            if (num < 0)
                return 10;
            int remain = num % 10;
            int result = num;
            if (remain <= 5)
                result -= remain;
            else
                result += (10 - remain);



            return result;

        }

        void DrawPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.isDesignMode)
            {

                this.ChangeCursor(e.Location);

                if (e.Button == System.Windows.Forms.MouseButtons.Left)
                {
                    if (isControlMoving)
                    {
                        Point NewPoint = new Point(NumberChanged(e.Location.X),
                            NumberChanged(e.Location.Y));
                        MoveControl(NewPoint);
                    }
                    else if (isControlChanging)
                    {
                        ChangeSelectedControlSize(e.Location);
                    }
                }
            }
        }
        

        void DrawPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (this.isDesignMode)
            {
                if (e.Button == System.Windows.Forms.MouseButtons.Left)
                {
                    MouseLeftClick(this, new MouseDownEventArgs(selectedControl));
                    Control control = this.FindControl(e.Location);
                    if (this.selectedControlViewport.CurrentCusorStatus == CusorStatus.None ||
                        this.selectedControlViewport.CurrentCusorStatus == CusorStatus.SizeAll)
                    {
                        this.SelectedControl = control;
                    }

                    if (this.selectedControl != null)
                    {
                        isControlChanging = true;
                        childControlClickedPoint = selectedControl.PointToClient(MousePosition);
                    }
                    if (this.selectedControlViewport.CurrentCusorStatus == CusorStatus.SizeAll)
                    {
                        isControlMoving = true;
                    }
                }
                else if (e.Button == System.Windows.Forms.MouseButtons.Right)
                {
                    if (this.selectedControl != null && this.selectedControl.Bounds.Contains(e.Location))
                    {
                        if (MouseRightClick != null)
                        {
                            MouseRightClick(this, new MouseDownEventArgs(selectedControl));
                        }
                    }
                }
            }
        }

        private void MoveControl(Point point)
        {
            InvalidateSelectedObject();

            point.Offset(-childControlClickedPoint.X, -childControlClickedPoint.Y);
            if (selectedControl == null)
                return;
            selectedControl.Location = point;
            
            InvalidateSelectedObject();
        }

       

        void DrawPanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (this.isDesignMode)
            {
                if (this.isControlChanging)
                {
                    this.isControlChanging = false;
                    this.InvalidateSelectedObject();
                }
                isControlMoving = false;
            }
        }
        

        private Control FindControl(Point point)
        {
            Control ctl = null;
            for (int i = 0; i < this.Controls.Count; i++)
            {
                Control iter = this.Controls[i];
                Rectangle rect = iter.Bounds;
                rect.Inflate(SelectedControlViewport.DRAG_HANDLE_SIZE, SelectedControlViewport.DRAG_HANDLE_SIZE);
                if (iter != transparentPanel && rect.Contains(point))
                {
                    ctl = iter;
                    break;
                }
            }
            return ctl;
        }

        private void ChangeSelectedControlSize(Point pos)
        {
         
            this.InvalidateSelectedObject();

            this.selectedControlViewport.ChangeSelectedControlBounds(selectedControl, pos);
            
      
            this.InvalidateSelectedObject();
        }


        private void ChangeCursor(Point point)
        {
            Control control = this.FindControl(point);
 
            if (control != null && this.isControlChanging == false)
            {
                Cursor = Cursors.SizeAll;
            }
            else
            {
                Cursor = Cursors.Default;
            }

         
            if (this.selectedControl != null && this.isControlChanging == false)
            {
                Point pos = this.PointToClient(MousePosition);
                Cursor cursor = this.selectedControlViewport.GetCursor(this.selectedControl, pos);
                if (cursor != null)
                    this.Cursor = cursor;
            }

        }


        void DrawPanel_ControlAdded(object sender, ControlEventArgs e)
        {
            Control ctl = e.Control;
            ctl.Paint -= OnPaint;
            ctl.Paint += OnPaint;

            this.SelectedControl = ctl;
        }


        void DrawPanel_ControlRemoved(object sender, ControlEventArgs e)
        {
            if (this.selectedControl == e.Control)
            {
                this.SelectedControl = null;
            }
        }


        private void DrawPanel_DragOver(object sender, DragEventArgs e)
        {
            TreeNode dragTreeNode = e.Data.GetData(typeof(TreeNode)) as TreeNode;
            if (dragTreeNode != null && dragTreeNode.Tag != null)
            {
                e.Effect = DragDropEffects.Move;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }


        private void DrawPanel_DragDrop(object sender, DragEventArgs e)
        {
            TreeNode dragTreeNode = e.Data.GetData(typeof(TreeNode)) as TreeNode;
            if (dragTreeNode != null && dragTreeNode.Tag != null)
            {
                Control control = dragTreeNode.Tag as Control;
                Point screenPoint = new Point(e.X, e.Y);
                Point clientPoint = this.PointToClient(screenPoint);
                AddControl(control, clientPoint);
            }
        }

        private void AddControl(Control control, Point point)
        {
            control.Location = point;
            this.Controls.Add(control);
        }
        

     
        public void OnPaint(object sender, PaintEventArgs e)
        {
            this.transparentPanel.BringToFront();

            Control control = sender as Control;

            control.Location = new Point(NumberChanged(control.Location.X),
                NumberChanged(control.Location.Y));
            Point location;

            if(control != this)
            {
              
                location = this.PointToClient(control.Parent.PointToScreen(control.Location));

          
                location += new Size((control.Width - control.ClientSize.Width) / 2, (control.Height - control.ClientSize.Height) / 2);

                e.Graphics.TranslateTransform(-location.X, -location.Y);
            }

      
            if (DesignMode == false && this.IsDrawSelectedControlBorder())
            {
                selectedControlViewport.Draw(e.Graphics, this.selectedControl);
            }

            if (Painted != null)
                Painted(sender, e);
        }
    }

    public class MouseDownEventArgs : EventArgs
    {
        public Control Control { get; private set; }
        public MouseDownEventArgs(Control ctl)
        {
            this.Control = ctl;
        }
    }

    public delegate void MouseDownEventHandler(object sender, MouseDownEventArgs args);

}
