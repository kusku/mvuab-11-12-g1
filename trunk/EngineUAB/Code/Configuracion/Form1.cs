using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace Configuracion
{
    public partial class Form1 : Form
    {
        struct Resol
        {
            public int Width;
            public int Height;

            public Resol(int width, int height)
            {
                Width = width;
                Height = height;
            }

            public override bool Equals(object obj)
            {
                if (obj.GetType() == typeof(Resol))
                {
                    Resol oth = (Resol)obj;
                    
                    if (this.Width == oth.Width && this.Height == oth.Height)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }

                return base.Equals(obj);
            }

            public override int GetHashCode()
            {
                return base.GetHashCode();
            }
        };


        Dictionary<Resol, Resol> resolDic;
        Dictionary<Resol, int> wrDic;
        Dictionary<int, Resol> wrRevDic;
        Dictionary<int, Resol> ShadowDic;
        Dictionary<Resol, int> ShadowRevDic;


        public Form1()
        {
            InitializeComponent();

            InitValues();

            InitConfig();


        }

        private void InitValues()
        {
            wrDic = new Dictionary<Resol, int>();
            wrDic.Add(new Resol(640, 360), 0);
            wrDic.Add(new Resol(800, 600), 1);
            wrDic.Add(new Resol(1024, 768), 2);
            wrDic.Add(new Resol(1280, 720), 3);
            wrDic.Add(new Resol(1280, 1024), 4);
            wrDic.Add(new Resol(1920, 1080), 5);

            wrRevDic = new Dictionary<int, Resol>();
            wrRevDic.Add(0, new Resol(640, 360)); 
            wrRevDic.Add(1, new Resol(800, 600));
            wrRevDic.Add(2, new Resol(1024, 768));
            wrRevDic.Add(3, new Resol(1280, 720));
            wrRevDic.Add(4, new Resol(1280, 1024));
            wrRevDic.Add(5, new Resol(1920, 1080));

            resolDic = new Dictionary<Resol, Resol>();
            resolDic.Add(new Resol(640, 360), new Resol(640, 360));
            resolDic.Add(new Resol(800, 600), new Resol(800, 450));
            resolDic.Add(new Resol(1024, 768), new Resol(1024, 576));
            resolDic.Add(new Resol(1280, 720), new Resol(1280, 720));
            resolDic.Add(new Resol(1280, 1024), new Resol(1280, 720));
            resolDic.Add(new Resol(1920, 1080), new Resol(1920, 1080));

            ShadowDic = new Dictionary<int, Resol>();
            ShadowDic.Add(0, new Resol(512, 512));
            ShadowDic.Add(1, new Resol(1024, 1024));
            ShadowDic.Add(2, new Resol(2048, 2048));
            ShadowDic.Add(3, new Resol(4096, 4096));

            ShadowRevDic = new Dictionary<Resol, int>();
            ShadowRevDic.Add(new Resol(512, 512), 0);
            ShadowRevDic.Add(new Resol(1024, 1024), 1);
            ShadowRevDic.Add(new Resol(2048, 2048), 2);
            ShadowRevDic.Add(new Resol(4096, 4096), 3);

        }

        private void InitConfig()
        {
            XmlTextReader engineReader = new XmlTextReader("./Data/XML/engine.xml");
                        
            while (engineReader.Read())
            {
                if (engineReader.NodeType == XmlNodeType.Element && engineReader.Name == "WindowResolution")
                {
                    Resol resWR = new Resol();

                    while (engineReader.MoveToNextAttribute())
                    {
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "width")
                        {
                            resWR.Width = engineReader.ReadContentAsInt();
                        }
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "height")
                        {
                            resWR.Height = engineReader.ReadContentAsInt();
                        }
                    }

                    cbResolution.SelectedIndex = wrDic[resWR];
                }

                if (engineReader.NodeType == XmlNodeType.Element && engineReader.Name == "RenderMode")
                {
                    while (engineReader.MoveToNextAttribute())
                    {
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "fullscreenMode")
                        {
                            cbWindowMode.Checked = !engineReader.ReadContentAsBoolean();
                        }
                    }
                }

                if (engineReader.NodeType == XmlNodeType.Element && engineReader.Name == "GraphicOpt")
                {
                    while (engineReader.MoveToNextAttribute())
                    {
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "motionBlur")
                        {
                            cbMotionBlurEnable.Checked = engineReader.ReadContentAsBoolean();
                        }
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "zblur")
                        {
                            cbZBlurEnable.Checked = engineReader.ReadContentAsBoolean();
                        }
                    }
                }
            }

            engineReader.Close();

            engineReader = new XmlTextReader("./Data/XML/lights.xml");

            while (engineReader.Read())
            {
                if (engineReader.NodeType == XmlNodeType.Element && engineReader.Name == "directional")
                {
                    Resol resSQ = new Resol();
                    bool genShadow = false;

                    while (engineReader.MoveToNextAttribute())
                    {
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "generate_shadow_map")
                        {
                            genShadow = engineReader.ReadContentAsBoolean();
                        }
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "shadow_map_width")
                        {
                            resSQ.Width = engineReader.ReadContentAsInt();
                        }
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "shadow_map_height")
                        {
                            resSQ.Height = engineReader.ReadContentAsInt();
                        }
                    }

                    cbShadowQuality.SelectedIndex = ShadowRevDic[resSQ];

                    cbShadowEnable.Checked = genShadow;

                    break;
                }
            }

            engineReader.Close();
        }

        private void bApply_Click(object sender, EventArgs e)
        {
            WriteToEngine();

            WriteToLight();

            MessageBox.Show("Changes Applied Successfully");
        }

        private void WriteToLight()
        {
            XmlTextReader engineReader = new XmlTextReader("./Data/XML/lights.xml");

            engineReader.Read();

            XmlDocument xmlDoc = new XmlDocument();

            xmlDoc.Load(engineReader);

            engineReader.Close();

            XmlNode elem = xmlDoc.FirstChild;

            for (; elem != null; elem = elem.NextSibling)
            {
                if (elem.NodeType == XmlNodeType.Element && elem.Name == "lights")
                {
                    elem = elem.ChildNodes[0];
                }

                if (elem.NodeType == XmlNodeType.Element && elem.Name == "directional")
                {
                    Resol sqRes = ShadowDic[cbShadowQuality.SelectedIndex];
                    bool shadowEnable = cbShadowEnable.Checked;

                    elem.Attributes["generate_shadow_map"].InnerText = shadowEnable.ToString().ToLower();

                    elem.Attributes["shadow_map_width"].InnerText = sqRes.Width.ToString();
                    elem.Attributes["shadow_map_height"].InnerText = sqRes.Height.ToString();

                    break;
                }
            }

            XmlTextWriter engineWriter = new XmlTextWriter("./Data/XML/lights.xml", null);

            xmlDoc.WriteTo(engineWriter);

            engineWriter.Flush();

            engineWriter.Close();
        }

        private void WriteToEngine()
        {
            XmlTextReader engineReader = new XmlTextReader("./Data/XML/engine.xml");

            engineReader.Read();

            XmlDocument xmlDoc = new XmlDocument();

            xmlDoc.Load(engineReader);

            engineReader.Close();

            XmlNode elem = xmlDoc.FirstChild;
            Resol wrRes = wrRevDic[cbResolution.SelectedIndex];
            Resol wrScreen = resolDic[wrRes];

            for (; elem != null; elem = elem.NextSibling)
            {
                if (elem.NodeType == XmlNodeType.Element && elem.Name == "Config")
                {
                    elem = elem.ChildNodes[0];
                }

                if (elem.NodeType == XmlNodeType.Element && elem.Name == "WindowResolution")
                {
                    elem.Attributes["width"].InnerText = wrRes.Width.ToString();
                    elem.Attributes["height"].InnerText = wrRes.Height.ToString();
                }

                if (elem.NodeType == XmlNodeType.Element && elem.Name == "ScreenResolution")
                {
                    elem.Attributes["width"].InnerText = wrScreen.Width.ToString();
                    elem.Attributes["height"].InnerText = wrScreen.Height.ToString();
                }

                if (elem.NodeType == XmlNodeType.Element && elem.Name == "RenderMode")
                {
                    bool windowed = !cbWindowMode.Checked;

                    elem.Attributes["fullscreenMode"].InnerText = windowed.ToString().ToLower();
                }

                if (elem.NodeType == XmlNodeType.Element && elem.Name == "GraphicOpt")
                {
                    bool motionBlurEnable = cbMotionBlurEnable.Checked;
                    bool zblurEnable = cbZBlurEnable.Checked;

                    elem.Attributes["motionBlur"].InnerText = motionBlurEnable.ToString().ToLower();
                    elem.Attributes["zblur"].InnerText = zblurEnable.ToString().ToLower();
                }
            }

            XmlTextWriter engineWriter = new XmlTextWriter("./Data/XML/engine.xml", null);

            xmlDoc.WriteTo(engineWriter);

            engineWriter.Flush();

            engineWriter.Close();
        }

        private void bExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
