using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
namespace RoteRoteMapView
{
    class RoteObjects
    {

        private Point _point;
        public Point position
        {

            get
            {
                return _point;
            }
            set
            {
                _point = value;
            }
        }


        private string _name;
        public string name
        {
            get
            {
                return _name;

            }
            set
            {

                _name = value;

            }


        }


        public override string ToString()
        {
            return name;
        }

    }

}
