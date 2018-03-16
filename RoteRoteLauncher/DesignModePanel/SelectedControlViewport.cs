using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ControlDesignMode
{
    /// <summary>
    /// 커서 상태
    /// </summary>
    internal enum CusorStatus
    {
        None,
        NW,
        N,
        NE,
        W,
        E,
        SW,
        S,
        SE,
        SizeAll,
    }

    /// <summary>
    /// 컨트롤을 선택했을시의 영역에 관한 클래스
    /// </summary>
    internal class SelectedControlViewport
    {
        /// <summary>
        /// 최소 컨트롤 크기
        /// </summary>
        const int MinControlSize = 20;
        /// <summary>
        /// 크기 조절하는 Rectangle 크기
        /// </summary>
        public const int DRAG_HANDLE_SIZE = 7;
        /// <summary>
        /// 커서 상태
        /// </summary>
        CusorStatus cursorStatus = CusorStatus.None;
        /// <summary>
        /// 현재 커서 상태를 가져온다.
        /// </summary>
        public CusorStatus CurrentCusorStatus
        {
            get { return cursorStatus; }
        }

        public void Draw(Graphics g, Control selectedObject)
        {
            //define the border to be drawn, it will be offset by DRAG_HANDLE_SIZE / 2
            //around the control, so when the drag handles are drawn they will be seem
            //connected in the middle.
            Rectangle border = new Rectangle(
                new Point(selectedObject.Location.X - DRAG_HANDLE_SIZE / 2,
                    selectedObject.Location.Y - DRAG_HANDLE_SIZE / 2),
                new Size(selectedObject.Size.Width + DRAG_HANDLE_SIZE,
                    selectedObject.Size.Height + DRAG_HANDLE_SIZE));


            //define the 8 drag handles, that has the size of DRAG_HANDLE_SIZE
            Rectangle NW = new Rectangle(
                new Point(selectedObject.Location.X - DRAG_HANDLE_SIZE,
                    selectedObject.Location.Y - DRAG_HANDLE_SIZE),
                new Size(DRAG_HANDLE_SIZE, DRAG_HANDLE_SIZE));

            Rectangle N = new Rectangle(
                new Point(selectedObject.Location.X + selectedObject.Width / 2 - DRAG_HANDLE_SIZE / 2,
                    selectedObject.Location.Y - DRAG_HANDLE_SIZE),
                new Size(DRAG_HANDLE_SIZE, DRAG_HANDLE_SIZE));

            Rectangle NE = new Rectangle(
                new Point(selectedObject.Location.X + selectedObject.Width,
                    selectedObject.Location.Y - DRAG_HANDLE_SIZE),
                new Size(DRAG_HANDLE_SIZE, DRAG_HANDLE_SIZE));

            Rectangle W = new Rectangle(
                new Point(selectedObject.Location.X - DRAG_HANDLE_SIZE,
                    selectedObject.Location.Y + selectedObject.Height / 2 - DRAG_HANDLE_SIZE / 2),
                new Size(DRAG_HANDLE_SIZE, DRAG_HANDLE_SIZE));

            Rectangle E = new Rectangle(
                new Point(selectedObject.Location.X + selectedObject.Width,
                    selectedObject.Location.Y + selectedObject.Height / 2 - DRAG_HANDLE_SIZE / 2),
                new Size(DRAG_HANDLE_SIZE, DRAG_HANDLE_SIZE));

            Rectangle SW = new Rectangle(
                new Point(selectedObject.Location.X - DRAG_HANDLE_SIZE,
                    selectedObject.Location.Y + selectedObject.Height),
                new Size(DRAG_HANDLE_SIZE, DRAG_HANDLE_SIZE));

            Rectangle S = new Rectangle(
                new Point(selectedObject.Location.X + selectedObject.Width / 2 - DRAG_HANDLE_SIZE / 2,
                    selectedObject.Location.Y + selectedObject.Height),
                new Size(DRAG_HANDLE_SIZE, DRAG_HANDLE_SIZE));

            Rectangle SE = new Rectangle(
                new Point(selectedObject.Location.X + selectedObject.Width,
                    selectedObject.Location.Y + selectedObject.Height),
                new Size(DRAG_HANDLE_SIZE, DRAG_HANDLE_SIZE));

            //draw the border and drag handles
            ControlPaint.DrawBorder(g, border, Color.Gray, ButtonBorderStyle.Dotted);
            ControlPaint.DrawGrabHandle(g, NW, true, true);
            ControlPaint.DrawGrabHandle(g, N, true, true);
            ControlPaint.DrawGrabHandle(g, NE, true, true);
            ControlPaint.DrawGrabHandle(g, W, true, true);
            ControlPaint.DrawGrabHandle(g, E, true, true);
            ControlPaint.DrawGrabHandle(g, SW, true, true);
            ControlPaint.DrawGrabHandle(g, S, true, true);
            ControlPaint.DrawGrabHandle(g, SE, true, true);
        }

        /// <summary>
        /// 현재 커서를 반환한다.
        /// </summary>
        /// <param name="enteredControl"></param>
        /// <param name="pos"></param>
        /// <returns></returns>
        internal Cursor GetCursor(Control enteredControl, Point pos)
        {
            Cursor cursor = null;
            //마우스 위치에 있는 컨트롤과 선택한 컨트롤이 일치하면
            if ((pos.X >= enteredControl.Location.X - DRAG_HANDLE_SIZE &&
                pos.X <= enteredControl.Location.X) &&
                (pos.Y >= enteredControl.Location.Y - DRAG_HANDLE_SIZE &&
                pos.Y <= enteredControl.Location.Y))
            {
                cursorStatus = CusorStatus.NW;
                cursor = Cursors.SizeNWSE;
            }
            else if ((pos.X >= enteredControl.Location.X + enteredControl.Width &&
                pos.X <= enteredControl.Location.X + enteredControl.Width + DRAG_HANDLE_SIZE &&
                pos.Y >= enteredControl.Location.Y + enteredControl.Height &&
                pos.Y <= enteredControl.Location.Y + enteredControl.Height + DRAG_HANDLE_SIZE))
            {
                cursorStatus = CusorStatus.SE;
                cursor = Cursors.SizeNWSE;
            }
            else if ((pos.X >= enteredControl.Location.X + enteredControl.Width / 2 - DRAG_HANDLE_SIZE / 2) &&
                pos.X <= enteredControl.Location.X + enteredControl.Width / 2 + DRAG_HANDLE_SIZE / 2 &&
                pos.Y >= enteredControl.Location.Y - DRAG_HANDLE_SIZE &&
                pos.Y <= enteredControl.Location.Y)
            {
                cursorStatus = CusorStatus.N;
                cursor = Cursors.SizeNS;
            }
            else if ((pos.X >= enteredControl.Location.X + enteredControl.Width / 2 - DRAG_HANDLE_SIZE / 2) &&
                pos.X <= enteredControl.Location.X + enteredControl.Width / 2 + DRAG_HANDLE_SIZE / 2 &&
                pos.Y >= enteredControl.Location.Y + enteredControl.Height &&
                pos.Y <= enteredControl.Location.Y + enteredControl.Height + DRAG_HANDLE_SIZE)
            {
                cursorStatus = CusorStatus.S;
                cursor = Cursors.SizeNS;
            }
            else if ((pos.X >= enteredControl.Location.X - DRAG_HANDLE_SIZE &&
                pos.X <= enteredControl.Location.X &&
                pos.Y >= enteredControl.Location.Y + enteredControl.Height / 2 - DRAG_HANDLE_SIZE / 2 &&
                pos.Y <= enteredControl.Location.Y + enteredControl.Height / 2 + DRAG_HANDLE_SIZE / 2))
            {
                cursorStatus = CusorStatus.W;
                cursor = Cursors.SizeWE;
            }
            else if ((pos.X >= enteredControl.Location.X + enteredControl.Width &&
                pos.X <= enteredControl.Location.X + enteredControl.Width + DRAG_HANDLE_SIZE &&
                pos.Y >= enteredControl.Location.Y + enteredControl.Height / 2 - DRAG_HANDLE_SIZE / 2 &&
                pos.Y <= enteredControl.Location.Y + enteredControl.Height / 2 + DRAG_HANDLE_SIZE / 2))
            {
                cursorStatus = CusorStatus.E;
                cursor = Cursors.SizeWE;
            }
            else if ((pos.X >= enteredControl.Location.X + enteredControl.Width &&
                pos.X <= enteredControl.Location.X + enteredControl.Width + DRAG_HANDLE_SIZE) &&
                (pos.Y >= enteredControl.Location.Y - DRAG_HANDLE_SIZE &&
                pos.Y <= enteredControl.Location.Y))
            {
                cursorStatus = CusorStatus.NE;
                cursor = Cursors.SizeNESW;
            }
            else if ((pos.X >= enteredControl.Location.X - DRAG_HANDLE_SIZE &&
                pos.X <= enteredControl.Location.X + DRAG_HANDLE_SIZE) &&
                (pos.Y >= enteredControl.Location.Y + enteredControl.Height - DRAG_HANDLE_SIZE &&
                pos.Y <= enteredControl.Location.Y + enteredControl.Height + DRAG_HANDLE_SIZE))
            {
                cursorStatus = CusorStatus.SW;
                cursor = Cursors.SizeNESW;
            }
            else if (enteredControl.Bounds.Contains(pos))
            {
                cursorStatus = CusorStatus.SizeAll;
            }
            else
            {
                cursorStatus = CusorStatus.None;
            }
            
            return cursor;
        }

        /// <summary>
        /// 해당 선택한 개체의 크기 및 위치를 변경한다.
        /// </summary>
        /// <param name="selectedObject"></param>
        /// <param name="pos"></param>
        internal void ChangeSelectedControlBounds(Control selectedObject, Point pos)
        {
            //get the current mouse position relative the the app
            //Point pos = this.PointToClient(point);
            Point newLocation;
            Size newSize;
            bool isHeightSmall, isWidthSmall;
            if (cursorStatus == CusorStatus.NW)
            {
                newLocation = new Point(pos.X, pos.Y);
                newSize = new Size(selectedObject.Size.Width - (newLocation.X - selectedObject.Location.X),
                    selectedObject.Size.Height - (newLocation.Y - selectedObject.Location.Y));
                CheckControlMinimumSize(ref newSize, out isHeightSmall, out isWidthSmall);
                if (isHeightSmall)
                {
                    newLocation.Y = selectedObject.Bounds.Bottom - MinControlSize;
                }
                if (isWidthSmall)
                {
                    newLocation.X = selectedObject.Bounds.Right - MinControlSize;
                }
                selectedObject.Size = newSize;
                selectedObject.Location = newLocation;
            }
            else if (cursorStatus == CusorStatus.SE)
            {
                newLocation = new Point(pos.X, pos.Y);
                newSize = new Size(selectedObject.Size.Width + (newLocation.X - selectedObject.Size.Width - selectedObject.Location.X),
                    selectedObject.Height + (newLocation.Y - selectedObject.Height - selectedObject.Location.Y));
                CheckControlMinimumSize(ref newSize);
                selectedObject.Size = newSize;
            }
            else if (cursorStatus == CusorStatus.N)
            {
                newLocation = new Point(selectedObject.Location.X, pos.Y);
                newSize = new Size(selectedObject.Width,
                    selectedObject.Height - (pos.Y - selectedObject.Location.Y));
                CheckControlMinimumSize(ref newSize, out isHeightSmall, out isWidthSmall);
                if (isHeightSmall)
                {
                    newLocation.Y = selectedObject.Bounds.Bottom - MinControlSize;
                }
                if (isWidthSmall)
                {
                    newLocation.X = selectedObject.Bounds.Right - MinControlSize;
                }
                selectedObject.Size = newSize;
                selectedObject.Location = newLocation;
            }
            else if (cursorStatus == CusorStatus.S)
            {
                newLocation = new Point(pos.X, pos.Y);
                newSize = new Size(selectedObject.Width,
                    pos.Y - selectedObject.Location.Y);
                CheckControlMinimumSize(ref newSize);
                selectedObject.Size = newSize;
            }
            else if (cursorStatus == CusorStatus.W)
            {
                newLocation = new Point(pos.X, selectedObject.Location.Y);
                newSize = new Size(selectedObject.Width - (pos.X - selectedObject.Location.X),
                    selectedObject.Height);
                CheckControlMinimumSize(ref newSize, out isHeightSmall, out isWidthSmall);
                if (isHeightSmall)
                {
                    newLocation.Y = selectedObject.Bounds.Bottom - MinControlSize;
                }
                if (isWidthSmall)
                {
                    newLocation.X = selectedObject.Bounds.Right - MinControlSize;
                }
                selectedObject.Size = newSize;
                selectedObject.Location = newLocation;
            }
            else if (cursorStatus == CusorStatus.E)
            {
                newLocation = new Point(pos.X, pos.Y);
                newSize = new Size(pos.X - selectedObject.Location.X,
                    selectedObject.Height);
                CheckControlMinimumSize(ref newSize);
                selectedObject.Size = newSize;
            }
            else if (cursorStatus == CusorStatus.SW)
            {
                newLocation = new Point(pos.X, selectedObject.Location.Y);
                newSize = new Size(selectedObject.Width - (pos.X - selectedObject.Location.X),
                    pos.Y - selectedObject.Location.Y);
                CheckControlMinimumSize(ref newSize, out isHeightSmall, out isWidthSmall);
                if (isHeightSmall)
                {
                    newLocation.Y = selectedObject.Bounds.Top;
                }
                if (isWidthSmall)
                {
                    newLocation.X = selectedObject.Bounds.Right - MinControlSize;
                }
                selectedObject.Size = newSize;
                selectedObject.Location = newLocation;
            }
            else if (cursorStatus == CusorStatus.NE)
            {
                newLocation = new Point(selectedObject.Location.X, pos.Y);
                newSize = new Size(pos.X - selectedObject.Location.X,
                    selectedObject.Height - (pos.Y - selectedObject.Location.Y));
                CheckControlMinimumSize(ref newSize, out isHeightSmall, out isWidthSmall);
                if (isHeightSmall)
                {
                    newLocation.Y = selectedObject.Bounds.Bottom - MinControlSize;
                }
                if (isWidthSmall)
                {
                    newLocation.X = selectedObject.Bounds.Left;
                }
                selectedObject.Size = newSize;
                selectedObject.Location = newLocation;
            }
        }

        /// <summary>
        /// 컨트롤의 최소 크기를 검사하고, 그 이하이면 수정한다.
        /// </summary>
        /// <param name="newSize"></param>
        private void CheckControlMinimumSize(ref Size newSize, out bool isHeightSmall, out bool isWidthSmall)
        {
            isHeightSmall = false;
            isWidthSmall = false;
            if (newSize.Height < MinControlSize)
            {
                newSize.Height = MinControlSize;
                isHeightSmall = true;
            }
            if (newSize.Width < MinControlSize)
            {
                newSize.Width = MinControlSize;
                isWidthSmall = true;
            }
        }
        /// <summary>
        /// 컨트롤의 최소 크기를 검사하고, 그 이하이면 수정한다.
        /// </summary>
        /// <param name="newSize"></param>
        private void CheckControlMinimumSize(ref Size newSize)
        {
            if (newSize.Height < MinControlSize)
            {
                newSize.Height = MinControlSize;
            }
            if (newSize.Width < MinControlSize)
            {
                newSize.Width = MinControlSize;
            }
        }
    }
}
