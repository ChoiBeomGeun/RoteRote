using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace RoteRoteMapView
{
    class parameter
    {
  

        [CategoryAttribute("MultiCam"), DescriptionAttribute("카메라의 PageDelay를 설정합니다"), DisplayNameAttribute("PageDelay")]
        public int m_PageDelay
        {
            get;
            set;

        }
        [CategoryAttribute("MultiCam"), DescriptionAttribute("Capture 의 여부를 설정합니다"), DisplayNameAttribute("Capture")]
        public bool m_bCapture
        {
            get;
            set;
        }

        public void SetParems(int inPageDealy, bool inCapture)
        {
            m_PageDelay = inPageDealy;
            m_bCapture = inCapture;


        }
    }
}
