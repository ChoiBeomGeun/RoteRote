using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ControlDesignMode
{
    /// <summary>
    /// 디자인 모드 패널 컨트롤
    /// </summary>
    public class ControlDesignModePanel : Panel
    {
        /// <summary>
        /// 컨트롤이 이동중인지 여부
        /// </summary>
        bool isControlMoving = false;
        /// <summary>
        /// 투명한 패널
        /// </summary>
        /// <remarks>투명한 패널을 두지 않게 되면 버튼 같은 컨트롤들이 클릭 이벤트 발생시
        /// 동작해버리므로 투명한 패널을 위에 올린다.</remarks>
        TransparentPanel transparentPanel;
        /// <summary>
        /// 컨트롤을 선택했을시의 영역에 관한 클래스
        /// </summary>
        SelectedControlViewport selectedControlViewport = new SelectedControlViewport();
        /// <summary>
        /// 자식 컨트롤 클릭한 위치 (자식 컨트롤 좌표 기준)
        /// </summary>
        Point childControlClickedPoint;
        /// <summary>
        /// 컨트롤 크기 수정 및 이동 여부
        /// </summary>
        bool isControlChanging = false;

        bool isDesignMode = true;
        /// <summary>
        /// 디자인 모드 여부를 가져오거나 설정한다.
        /// </summary>
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
        /// <summary>
        /// 선택한 컨트롤
        /// </summary>
        Control selectedControl = null;
        /// <summary>
        /// 선택한 개체를 설정한다.
        /// </summary>
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
        /// <summary>
        /// 컨트롤을 오른쪽 마우스 단추를 누르면 발생한다.
        /// </summary>
        public event MouseDownEventHandler MouseRightClick;
        public event MouseDownEventHandler MouseLeftClick;
        /// <summary>
        /// 화면을 다 그렸다면 발생한다.
        /// </summary>
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

        

        /// <summary>
        /// 보더 패널을 초기화한다.
        /// </summary>
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

        /// <summary>
        /// 보더 패널은 제거한다.
        /// </summary>
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

        /// <summary>
        /// 선택한 컨트롤의 테두리 영역을 그려야하는지 여부를 가져온다.
        /// </summary>
        /// <returns></returns>
        private bool IsDrawSelectedControlBorder()
        {
            return isDesignMode && this.selectedControl != null && this.isControlChanging == false;
        }

        /// <summary>
        /// 투명한 패널의 마우스 단추를 놓으면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TransparentPanel_MouseUp(object sender, MouseEventArgs e)
        {
            Point thisCtlPoint = this.PointToClient(MousePosition);
            DrawPanel_MouseUp(sender, new MouseEventArgs(e.Button, e.Clicks, thisCtlPoint.X, thisCtlPoint.Y, e.Delta));
        }

        /// <summary>
        /// 투명한 패널의 마우스 단추를 누르면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TransparentPanel_MouseDown(object sender, MouseEventArgs e)
        {
            Point thisCtlPoint = this.PointToClient(MousePosition);
            DrawPanel_MouseDown(sender, new MouseEventArgs(e.Button, e.Clicks, thisCtlPoint.X, thisCtlPoint.Y, e.Delta));
        }

        /// <summary>
        /// 투명한 패널의 마우스 포인터를 이동시키면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TransparentPanel_MouseMove(object sender, MouseEventArgs e)
        {
            Point thisCtlPoint = this.PointToClient(MousePosition);
            DrawPanel_MouseMove(sender, new MouseEventArgs(e.Button, e.Clicks, thisCtlPoint.X, thisCtlPoint.Y ,e.Delta));
        }

        /// <summary>
        /// 선택한 개체의 영역을 가져온다.
        /// </summary>
        /// <returns></returns>
        private Rectangle GetSelectedObjectRect()
        {
            Rectangle rect = this.selectedControl.Bounds;
            rect.Inflate(SelectedControlViewport.DRAG_HANDLE_SIZE, SelectedControlViewport.DRAG_HANDLE_SIZE);
            return rect;
        }

        /// <summary>
        /// 선택한 개체 영역을 다시 그린다.
        /// </summary>
        private void InvalidateSelectedObject()
        {
            if (this.selectedControl != null)
            {
                this.Invalidate(GetSelectedObjectRect(), true);
            }
        }
        public int NumberChanged(int num)
        {
            bool IsMinus = (num < 0) ? true : false;
            string number = num.ToString();

            int Lastnumber = Int32.Parse(number[number.Length - 1].ToString());


            if (!IsMinus)
                return num - Lastnumber;
            else
                return num + Lastnumber;

        }
        /// <summary>
        /// 마우스가 이동하면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void DrawPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.isDesignMode)
            {
                //이동 중에 컨트롤이 있으면 이동 커서로 변경
                //만약 컨트롤 선택이 되어 있고 모서리라면 해당 커서 변경
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
        
        /// <summary>
        /// 마우스 단추를 누르면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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

        /// <summary>
        /// 컨트롤을 이동시킨다.
        /// </summary>
        /// <param name="point"></param>
        private void MoveControl(Point point)
        {
            InvalidateSelectedObject();

            point.Offset(-childControlClickedPoint.X, -childControlClickedPoint.Y);
            if (selectedControl == null)
                return;
            selectedControl.Location = point;
            
            InvalidateSelectedObject();
        }

       
        /// <summary>
        /// 마우스 단추를 놓으면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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
        
        /// <summary>
        /// 해당 위치의 컨트롤을 검색한다.
        /// </summary>
        /// <param name="point"></param>
        /// <returns></returns>
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
        
        /// <summary>
        /// 선택한 컨트롤의 크기를 변경한다.
        /// </summary>
        /// <param name="point"></param>
        private void ChangeSelectedControlSize(Point pos)
        {
            //변경 전 위치 다시 그리기
            this.InvalidateSelectedObject();

            this.selectedControlViewport.ChangeSelectedControlBounds(selectedControl, pos);
            
            //변경 후 위치 다시 그리기
            this.InvalidateSelectedObject();
        }

        /// <summary>
        /// 현재 마우스 지점의 커서를 설정한다.
        /// </summary>
        /// <returns></returns>
        private void ChangeCursor(Point point)
        {
            Control control = this.FindControl(point);
            //마우스 위치에 컨트롤이 존재하고 컨트롤이 변경 중이 아니라면
            if (control != null && this.isControlChanging == false)
            {
                Cursor = Cursors.SizeAll;
            }
            else
            {
                Cursor = Cursors.Default;
            }

            //선택한 컨트롤이 존재하고 컨트롤이 변경 중이 아니라면
            if (this.selectedControl != null && this.isControlChanging == false)
            {
                Point pos = this.PointToClient(MousePosition);
                Cursor cursor = this.selectedControlViewport.GetCursor(this.selectedControl, pos);
                if (cursor != null)
                    this.Cursor = cursor;
            }

        }

        /// <summary>
        /// 컨트롤이 추가되면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void DrawPanel_ControlAdded(object sender, ControlEventArgs e)
        {
            Control ctl = e.Control;
            ctl.Paint -= OnPaint;
            ctl.Paint += OnPaint;

            this.SelectedControl = ctl;
        }

        /// <summary>
        /// 컨트롤이 제거되면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void DrawPanel_ControlRemoved(object sender, ControlEventArgs e)
        {
            if (this.selectedControl == e.Control)
            {
                this.SelectedControl = null;
            }
        }

        /// <summary>
        /// 그리는 패널에 Drag Over 되면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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

        /// <summary>
        /// 그리는 패널에 Drag Drop 되면 발생한다.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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

        /// <summary>
        /// 해당 위치에 컨트롤을 생성한다.
        /// </summary>
        /// <param name="ctlType"></param>
        /// <param name="point"></param>
        private void AddControl(Control control, Point point)
        {
            control.Location = point;
            this.Controls.Add(control);
        }
        
        /// <summary>
        /// 화면을 그려야할때 발생한다.
        /// </summary>
        /// <remarks>선택 영역을 최상위로 그리기 위해 자식 컨트롤들이 그려진 이후에 그려야한다.
        /// (맨 마지막에 그려지는 자식 컨트롤을 알면 그녀석만 등록하면 더좋을것같다)
        /// </remarks>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void OnPaint(object sender, PaintEventArgs e)
        {
            this.transparentPanel.BringToFront();

            Control control = sender as Control;

            control.Location = new Point(NumberChanged(control.Location.X),
                NumberChanged(control.Location.Y));
            Point location;

            if(control != this)
            {
                // The control may be in a hierarchy, so convert to screen coordinates and then back to form coordinates.
                location = this.PointToClient(control.Parent.PointToScreen(control.Location));

                // If the control has a border shift the location of the control's client area.
                location += new Size((control.Width - control.ClientSize.Width) / 2, (control.Height - control.ClientSize.Height) / 2);

                e.Graphics.TranslateTransform(-location.X, -location.Y);
            }

            //Draw
            if (DesignMode == false && this.IsDrawSelectedControlBorder())
            {
                selectedControlViewport.Draw(e.Graphics, this.selectedControl);
            }

            if (Painted != null)
                Painted(sender, e);
        }
    }

    /// <summary>
    /// 컨트롤을 마우스 단추를 누르면 발생한다.
    /// </summary>
    public class MouseDownEventArgs : EventArgs
    {
        public Control Control { get; private set; }
        public MouseDownEventArgs(Control ctl)
        {
            this.Control = ctl;
        }
    }
    /// <summary>
    /// 컨트롤을 마우스 단추를 누르면 발생한다.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="args"></param>
    public delegate void MouseDownEventHandler(object sender, MouseDownEventArgs args);

}
