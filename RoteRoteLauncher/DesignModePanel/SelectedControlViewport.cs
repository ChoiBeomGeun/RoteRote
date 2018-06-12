/******************************************************************************/
/*!
\file   SelectedControlViewport.cs
\author Choi Beom Geun
\par    email: o77151@gmail.com
\par    Class:GAM250
\par    RoteRoteMapEditor
\date   2018/6/6

Actions of Selected Objects
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

   
    internal class SelectedControlViewport
    {
      
        const int MinControlSize = 20;

        public const int DRAG_HANDLE_SIZE = 7;
 
        CusorStatus cursorStatus = CusorStatus.None;

        public CusorStatus CurrentCusorStatus
        {
            get { return cursorStatus; }
        }
        public int NumberChanged(int num)
        {
            if (num < 0)
                return 10;
            int remain = num % 10;
            int result =num;
            if (remain <= 5)
                result -= remain;
            else
                result += (10 - remain);



            return result;

        }
        public void Draw(Graphics g, Control selectedObject)
        {
        
            Rectangle border = new Rectangle(
                new Point(selectedObject.Location.X - DRAG_HANDLE_SIZE / 2,
                    selectedObject.Location.Y - DRAG_HANDLE_SIZE / 2),
                new Size(selectedObject.Size.Width + DRAG_HANDLE_SIZE,
                    selectedObject.Size.Height + DRAG_HANDLE_SIZE));


      
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


        internal Cursor GetCursor(Control enteredControl, Point pos)
        {
            Cursor cursor = null;
 
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


        internal void ChangeSelectedControlBounds(Control selectedObject, Point pos)
        {
      
            Point newLocation;
            Size newSize;
            bool isHeightSmall, isWidthSmall;
           
            if (cursorStatus == CusorStatus.NW)
            {
                newLocation = new Point(pos.X, pos.Y);
                
                newLocation = new Point(NumberChanged(newLocation.X), NumberChanged(newLocation.Y));
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

                newSize = new Size(NumberChanged(newSize.Width), NumberChanged(newSize.Height));
                
                selectedObject.Size = newSize;
                selectedObject.Location = newLocation;
            }
            else if (cursorStatus == CusorStatus.SE)
            {
                newLocation = new Point(pos.X, pos.Y);
                newLocation = new Point(NumberChanged(newLocation.X), NumberChanged(newLocation.Y));
                newSize = new Size(selectedObject.Size.Width + (newLocation.X - selectedObject.Size.Width - selectedObject.Location.X),
                    selectedObject.Height + (newLocation.Y - selectedObject.Height - selectedObject.Location.Y));
                
                CheckControlMinimumSize(ref newSize);
                newSize = new Size(NumberChanged(newSize.Width), NumberChanged(newSize.Height));

                selectedObject.Size = newSize;
            }
            else if (cursorStatus == CusorStatus.N)
            {
                newLocation = new Point(selectedObject.Location.X, pos.Y);
                newLocation = new Point(NumberChanged(newLocation.X), NumberChanged(newLocation.Y));
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
                newSize = new Size(NumberChanged(newSize.Width), NumberChanged(newSize.Height));
                
                selectedObject.Size = newSize;
                selectedObject.Location = newLocation;
            }
            else if (cursorStatus == CusorStatus.S)
            {
                newLocation = new Point(pos.X, pos.Y);
                newLocation = new Point(NumberChanged(newLocation.X), NumberChanged(newLocation.Y));
                newSize = new Size(selectedObject.Width,
                    pos.Y - selectedObject.Location.Y);

               
                CheckControlMinimumSize(ref newSize);
                newSize = new Size(NumberChanged(newSize.Width), NumberChanged(newSize.Height));
               
                selectedObject.Size = newSize;
            }
            else if (cursorStatus == CusorStatus.W)
            {
                newLocation = new Point(pos.X, selectedObject.Location.Y);
                newLocation = new Point(NumberChanged(newLocation.X), NumberChanged(newLocation.Y));
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
                newSize = new Size(NumberChanged(newSize.Width), NumberChanged(newSize.Height));
               
                selectedObject.Size = newSize;
                selectedObject.Location = newLocation;
            }
            else if (cursorStatus == CusorStatus.E)
            {
                newLocation = new Point(pos.X, pos.Y);
                newLocation = new Point(NumberChanged(newLocation.X), NumberChanged(newLocation.Y));
                newSize = new Size(pos.X - selectedObject.Location.X,
                    selectedObject.Height);
                
                CheckControlMinimumSize(ref newSize);


                newSize = new Size(NumberChanged(newSize.Width), NumberChanged(newSize.Height));
               
                selectedObject.Size = newSize;
            }
            else if (cursorStatus == CusorStatus.SW)
            {
                newLocation = new Point(pos.X, selectedObject.Location.Y);
                newLocation = new Point(NumberChanged(newLocation.X), NumberChanged(newLocation.Y));
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
                newSize = new Size(NumberChanged(newSize.Width), NumberChanged(newSize.Height));
                
                selectedObject.Size = newSize;
                selectedObject.Location = newLocation;
            }
            else if (cursorStatus == CusorStatus.NE)
            {
                newLocation = new Point(selectedObject.Location.X, pos.Y);
                newLocation = new Point(NumberChanged(newLocation.X), NumberChanged(newLocation.Y));
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

                               
                newSize = new Size(NumberChanged(newSize.Width), NumberChanged(newSize.Height));
                selectedObject.Size = newSize;
                selectedObject.Location = newLocation;
            }
        }


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
