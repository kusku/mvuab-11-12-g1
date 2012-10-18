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
        Dictionary<int, int> ParticleDic;
        Dictionary<int, int> ParticleRevDic;


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
            wrDic.Add(new Resol(1360, 768), 5);
            wrDic.Add(new Resol(1366, 768), 6);
            wrDic.Add(new Resol(1920, 1080), 7);

            wrRevDic = new Dictionary<int, Resol>();
            wrRevDic.Add(0, new Resol(640, 360)); 
            wrRevDic.Add(1, new Resol(800, 600));
            wrRevDic.Add(2, new Resol(1024, 768));
            wrRevDic.Add(3, new Resol(1280, 720));
            wrRevDic.Add(4, new Resol(1280, 1024));
            wrRevDic.Add(5, new Resol(1360, 768));
            wrRevDic.Add(6, new Resol(1366, 768));
            wrRevDic.Add(7, new Resol(1920, 1080));

            resolDic = new Dictionary<Resol, Resol>();
            resolDic.Add(new Resol(640, 360), new Resol(640, 360));
            resolDic.Add(new Resol(800, 600), new Resol(800, 450));
            resolDic.Add(new Resol(1024, 768), new Resol(1024, 576));
            resolDic.Add(new Resol(1280, 720), new Resol(1280, 720));
            resolDic.Add(new Resol(1280, 1024), new Resol(1280, 720));
            resolDic.Add(new Resol(1360, 768), new Resol(1360, 768));
            resolDic.Add(new Resol(1366, 768), new Resol(1366, 768));
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

            ParticleDic = new Dictionary<int, int>();
            ParticleDic.Add(0, 3);
            ParticleDic.Add(1, 2);
            ParticleDic.Add(2, 1);

            ParticleRevDic = new Dictionary<int, int>();
            ParticleRevDic.Add(3, 0);
            ParticleRevDic.Add(2, 1);
            ParticleRevDic.Add(1, 2);
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
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "bloom")
                        {
                            cbBloomEnable.Checked = engineReader.ReadContentAsBoolean();
                        }
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "particle_level")
                        {
                            cbParticleQuality.SelectedIndex = ParticleRevDic[engineReader.ReadContentAsInt()];
                        }
                        if (engineReader.NodeType == XmlNodeType.Attribute && engineReader.Name == "cs")
                        {
                            cbSettings.SelectedIndex = engineReader.ReadContentAsInt();
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
                    elem.Attributes["bloom"].InnerText = cbBloomEnable.Checked.ToString().ToLower();
                    elem.Attributes["particle_level"].InnerText = ParticleDic[cbParticleQuality.SelectedIndex].ToString();
                    elem.Attributes["cs"].InnerText = cbSettings.SelectedIndex.ToString();
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

        private void cbShadowEnable_CheckedChanged(object sender, EventArgs e)
        {
            cbSettings.SelectedIndex = 4;

            if (cbShadowEnable.Checked)
            {
                return;
            }

            cbShadowQuality.SelectedIndex = 0;
        }

        private void cbSettings_SelectedIndexChanged(object sender, EventArgs e)
        {
            int idx = cbSettings.SelectedIndex;

            switch (idx)
            {
                case 0:
                    //Shadows
                    cbShadowEnable.Checked = false;
                    cbShadowQuality.SelectedIndex = 0;

                    //Post Process
                    cbMotionBlurEnable.Checked = false;
                    cbZBlurEnable.Checked = false;
                    cbBloomEnable.Checked = false;

                    //Particles
                    cbParticleQuality.SelectedIndex = 0;
                    break;
                case 1:
                    //Shadows
                    cbShadowEnable.Checked = true;
                    cbShadowQuality.SelectedIndex = 1;

                    //Post Process
                    cbMotionBlurEnable.Checked = true;
                    cbZBlurEnable.Checked = true;
                    cbBloomEnable.Checked = false;

                    //Particles
                    cbParticleQuality.SelectedIndex = 1;
                    break;
                case 2:
                    //Shadows
                    cbShadowEnable.Checked = true;
                    cbShadowQuality.SelectedIndex = 2;

                    //Post Process
                    cbMotionBlurEnable.Checked = true;
                    cbZBlurEnable.Checked = true;
                    cbBloomEnable.Checked = true;

                    //Particles
                    cbParticleQuality.SelectedIndex = 2;
                    break;
                case 3:
                    //Shadows
                    cbShadowEnable.Checked = true;
                    cbShadowQuality.SelectedIndex = 3;

                    //Post Process
                    cbMotionBlurEnable.Checked = true;
                    cbZBlurEnable.Checked = true;
                    cbBloomEnable.Checked = true;

                    //Particles
                    cbParticleQuality.SelectedIndex = 2;
                    break;
                default:
                    break;
            }

            cbSettings.SelectedIndex = idx;
        }

        private void cbShadowQuality_SelectedIndexChanged(object sender, EventArgs e)
        {
            cbSettings.SelectedIndex = 4;
        }

        private void cbMotionBlurEnable_CheckedChanged(object sender, EventArgs e)
        {
            cbSettings.SelectedIndex = 4;
        }

        private void cbZBlurEnable_CheckedChanged(object sender, EventArgs e)
        {
            cbSettings.SelectedIndex = 4;
        }

        private void cbBloomEnable_CheckedChanged(object sender, EventArgs e)
        {
            cbSettings.SelectedIndex = 4;
        }

        private void cbParticleQuality_SelectedIndexChanged(object sender, EventArgs e)
        {
            cbSettings.SelectedIndex = 4;
        }

        private void cbResolution_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
