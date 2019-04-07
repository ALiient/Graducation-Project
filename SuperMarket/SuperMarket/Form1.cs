using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks; 
using System.Windows.Forms;

using MySql.Data;
using MySql.Data.MySqlClient;       //引用MySql组件
using System.IO;
using System.IO.Ports;              //引用串口组件
using System.Threading;


namespace SuperMarket
{
    public partial class Form1 : Form
    {
        private MySqlConnection SqlConn;
        private String Data_Pack;
        private bool User = new bool();         //true:Manager false:Customer
        private bool Action_Add = false;        //添加控制位
        private bool Action_Delete = false;     //删除控制位
        private const String SYS_INO = ">>>>>>>[@ShopCart-System]# ";
        private System.Media.SoundPlayer sp = new System.Media.SoundPlayer();
        static  String sql_connect = "server=localhost;Database=supermarket;user id=root;password=123456";
        static  String sql_cmd = "select 商品条码, 商品名称, 商品价格 from goods";
        MySqlDataAdapter adapter = new MySqlDataAdapter(sql_cmd, sql_connect);
        DataTable p_dt = new DataTable();


        public Form1()
        {
            InitializeComponent();
            Connect_MySql();        //连接数据库
            bool Success = Connect_SerialPort();   //连接串口
            
            if(Success)
                Play_Tips("Wait");      //开机语音提示
        }


        private bool Connect_SerialPort()
        {
            serialPort1 = new SerialPort();       //初始化串口对象实例

            serialPort1.BaudRate = 9600;
            serialPort1.PortName = "COM17";
            serialPort1.DataBits = 8;
            serialPort1.StopBits = StopBits.One;
            serialPort1.Parity = Parity.None;
            Control.CheckForIllegalCrossThreadCalls = false;        //不检查跨线程调用是否合法
            serialPort1.DataReceived += new SerialDataReceivedEventHandler(Usart_DataReceived);
            serialPort1.DtrEnable = true;
            serialPort1.RtsEnable = true;
            serialPort1.ReadTimeout = 1000;


            try
            {
                serialPort1.Open();
                serialPort1.DiscardInBuffer();
                richTextBox1.Text += SYS_INO + "串口连接完成，系统初始化成功！\n";
                return true;
            }
            catch (Exception ex)
            {
                Play_Tips("Usart_Error");       //语音提示“串口连接失败，请检查串口配置”
                richTextBox1.Text += SYS_INO + "串口连接失败，请检查串口配置！\n";
                return false;
            }

        }

        private void Connect_MySql()
        {
            String connectString = "server=localhost;Database=supermarket;user id=root;password=123456";     //数据库部署信息
            richTextBox1.Text += SYS_INO + "部署数据库<Supermarket>...\n";
            SqlConn = new MySqlConnection(connectString);
            richTextBox1.Text += SYS_INO + "数据库<Supermarket>连接成功！\n";


            User = false;        //Customer
            if (User)
            {
                label4.Text = "Manager";
                button3.BackgroundImage = Image.FromFile(@"E:\毕业论文\Something\Project\SuperMarket\Manager.png");
            }
            else
            {
                label4.Text = "Customer";
                button3.BackgroundImage = Image.FromFile(@"E:\毕业论文\Something\Project\SuperMarket\Customer.png");
            }

            //读取数据库数据至表格
            //MySqlDataAdapter adapter = new MySqlDataAdapter(sql_cmd, sql_connect);
            //DataTable p_dt = new DataTable();
            adapter.Fill(p_dt);
            this.dataGridView1.DataSource = p_dt;
        }


        private bool OpenConnection()                   //打开数据库
        {
            try
            {
                SqlConn.Open();                         //打开数据库
                return true;
            }
            catch (MySqlException ex)                   //捕获相应异常
            {
                switch (ex.Number)
                {
                    case 0: MessageBox.Show("连接数据库失败，请检查数据库部署！"); break;
                    case 1045: MessageBox.Show("无效的用户名或密码！"); break;
                }

                return false;
            }
        }

        private bool CloseConnection()                  //关闭数据库
        {
            try
            {
                SqlConn.Close();
                return true;
            }
            catch (MySqlException ex)
            {
                MessageBox.Show(ex.Message);
                return false;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            String insertString;
            insertString = "insert into Goods(商品条码, 商品名称, 商品价格) Values('" + label2.Text + "', '" + textBox1.Text + "', '" +textBox2.Text+ "')";

            if (User)                //Manager才有添加权限
            {
                if (Action_Add)     //需要识别到条码后，才可执行添加操作
                {
                    if (textBox1.Text == "" || textBox2.Text == "")
                    {
                        Play_Tips("Tip");                                                   //语音提示“填写完整信息”
                        richTextBox1.Text += SYS_INO + "请填写完整的商品信息！\n";
                        this.richTextBox1.SelectionStart = this.richTextBox1.TextLength;
                        this.richTextBox1.ScrollToCaret();                                  //让滚动条随数据下移
                    }
                    else
                    {
                        if (this.OpenConnection() == true)  //打开数据库连接
                        {
                            MySqlCommand myCmd = new MySqlCommand(insertString, SqlConn);
                            myCmd.ExecuteNonQuery();        //执行sql命令
                            this.CloseConnection();         //执行完，关闭数据库连接 

                            p_dt.Clear();                   //清空行
                            adapter.Fill(p_dt);
                            this.dataGridView1.DataSource = p_dt;

                            Play_Tips("Add");               //语音提示“添加成功”
                            richTextBox1.Text += SYS_INO + "添加" + textBox1.Text + "成功！\n";
                            Action_Add = false;             //执行完添加操作，重置操作位
                            label2.Text = "";
                            pictureBox4.Visible = false;
                            pictureBox5.Visible = true;
                            textBox1.Text = "";
                            textBox2.Text = "";             //清空编辑框
                        }
                    }
                }
                else if (Action_Delete)
                {
                    Play_Tips("Delete_Goods");              // 提示是否删除
                    richTextBox1.Text += SYS_INO + "已经有这个商品,要删除它吗？\n";
                }
                else
                {
                    Play_Tips("Tips_Check_Code");      //语音提示“请先识别商品条码”
                    richTextBox1.Text += SYS_INO + "请先识别商品条码！\n";
                }
            }
            else            //Customer无添加权限，提示切换权限
            {
                Play_Tips("No_Power_Add");      //语音提示“当前用户没有权限添加商品”
                richTextBox1.Text += SYS_INO + "当前用户没有权限添加商品！\n";
            }


        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            DateTime dt = DateTime.Now;
            label1.Text = dt.ToString();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Interval = 1000;
            timer1.Start();
        }


        private bool Check_Goods(String Code)
        {
            String Cmd_String = "select count(*) from goods " + "where 商品条码 = '" + Code + "' limit 1";
            MySqlCommand Cmd = new MySqlCommand(Cmd_String, SqlConn);   //执行查询命令
                                                                        //int reader = (int)Cmd.ExecuteScalar();                      //读取查询结果

            if (this.OpenConnection() == true)  //打开数据库连接
            {
                if ((Cmd.ExecuteScalar()) == null)
                {
                    Play_Tips("DataBase_Empty");    //语音提示“数据库是空的”
                    richTextBox1.Text += SYS_INO + "数据库是空的！\n";
                    this.CloseConnection();         //执行完，关闭数据库连接 
                    return false;
                }
                else
                {
                    int reader = Convert.ToInt32(Cmd.ExecuteScalar());
                    this.CloseConnection();         //执行完，关闭数据库连接 
                    if (reader != 0)
                        return true;
                    else
                        return false;
                }
            }
            else
            {
                Play_Tips("DataBase_Error");    //语音提示“数据库链接失败”
                richTextBox1.Text += SYS_INO + "数据库链接失败！\n";
                this.CloseConnection();         //执行完，关闭数据库连接 
                return false;
            }

        }

        private void Play_Tips(String Tips)
        {
            sp.SoundLocation = @"E:\毕业论文\Something\Project\SuperMarket\" + Tips + ".wav";
            sp.Play();
        }

        private void Display_Code(String Code)
        {
            label2.Text = Code;                                            //显示条码信息
            Data_Pack = "";
            serialPort1.DiscardInBuffer();                                 //清空缓冲区  
            pictureBox4.Visible = true;
            pictureBox5.Visible = false;
        }


        private void Usart_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                System.Text.UTF8Encoding utf8 = new System.Text.UTF8Encoding();
                byte[] Rev_Data = new byte[serialPort1.BytesToRead];        //串口接收buffer
                serialPort1.Read(Rev_Data, 0, Rev_Data.Length);             //读取串口内容
                string Serial_String = Encoding.GetEncoding("GB2312").GetString(Rev_Data);

                this.Invoke((EventHandler)(delegate         //Invoke同步
                {
                    Data_Pack += Serial_String;
                    if (Data_Pack.Length == 10)
                    {
                        if ((Data_Pack[0] == 'A') && (Data_Pack[9] == 'B'))     //正确的数据包
                        {
                            String Goods_Code = Data_Pack.Substring(1, 8);      //截取数据包中的条码信息

                            if (Check_Goods(Goods_Code))                        //查询到存在的商品，Customer和Manager分别有不同的操作
                            {
                                if (User)  //当前用户为Manager
                                {
                                    Play_Tips("Delete_Goods");
                                    Display_Code(Goods_Code);                   // 提示是否删除
                                    richTextBox1.Text += SYS_INO + "已经有这个商品,要删除它吗？\n";

                                    Action_Delete = true;                       //删除动作满足
                                }
                                else       //当前用户为Customr
                                {
                                    Play_Tips("Buy_Goods");
                                    String Ack_Data = "A1B";
                                    byte[] byteArray = System.Text.Encoding.Default.GetBytes(Ack_Data);    //将字符缓冲区的数据转换为byte数组
                                    serialPort1.Write(byteArray, 0, 3);                                   //应答返回查询信息

                                    Display_Code(Goods_Code);                  //显示条码信息
                                    richTextBox1.Text += SYS_INO + "找到它了，把它买下嘛!\n";
                                }
                            }
                            else                                              //查询不到商品，Customer和Manager分别有不同的操作
                            {
                                if (User)  //Manager：提示是否添加新的商品
                                {
                                    Play_Tips("Check");
                                    richTextBox1.Text += SYS_INO + "识别到新的商品,是否添加？\n";
                                    Display_Code(Goods_Code);                  //显示条码信息
                                    Action_Add = true;                         //添加操作满足
                                }
                                else      //Customer:找不到商品,应答查不到
                                {
                                    Play_Tips("No_Goods");
                                    Display_Code(Goods_Code);                  //显示条码信息
                                    richTextBox1.Text += SYS_INO + "好像没有这个商品喔，你换一个吧!\n";
                                }
                            }
                        } //错误或丢帧数据包，直接返回
                        else
                        {
                            Play_Tips("Check_Error");       //语音提示“识别失败”
                            richTextBox1.Text += SYS_INO + "识别失败!\n";
                            return;
                        }


                    }
                }));
            }
            else        
            {
                Play_Tips("Usart_Error");       //语音提示“串口连接失败，请检查串口配置”
                richTextBox1.Text += SYS_INO + "串口连接失败，请检查串口配置！\n";
            }

        }

        private void button1_Click(object sender, EventArgs e)
        {
            String Delete_String;
            Delete_String = "delete from Goods where 商品条码 = '" + label2.Text + "'";

            if (User)           //Manager才有删除权限
            {
                if (Action_Delete)      //需要识别到条码，才可以执行删除操作
                {
                    if (this.OpenConnection() == true)  //打开数据库连接
                    {
                        MySqlCommand myCmd = new MySqlCommand(Delete_String, SqlConn);
                        myCmd.ExecuteNonQuery();        //执行sql命令
                        this.CloseConnection();         //执行完，关闭数据库连接 
                        
                        p_dt.Clear();                   //清空行
                        adapter.Fill(p_dt);
                        this.dataGridView1.DataSource = p_dt;

                        Play_Tips("Delete_OK");               //语音提示“删除成功”
                        richTextBox1.Text += SYS_INO + "删除成功\n";
                        Action_Delete = false;      //执行完删除操作，重置操作位
                        label2.Text = "";
                        pictureBox4.Visible = false;
                        pictureBox5.Visible = true;
                        textBox1.Text = "";
                        textBox2.Text = "";             //清空编辑框
                    }
                }
                else if (Action_Add)
                {
                    Play_Tips("Check");
                    richTextBox1.Text += SYS_INO + "识别到新的商品,是否添加？\n";
                }
                else
                {
                    Play_Tips("Tips_Check_Code");      //语音提示“请先识别商品条码”
                    richTextBox1.Text += SYS_INO + "请先识别商品条码！\n";
                }
            }
            else                //语音提示“当前用户没有权限删除商品”
            {
                Play_Tips("No_Power_Delete");      //语音提示“当前用户没有权限删除商品”
                richTextBox1.Text += SYS_INO + "当前用户没有权限删除商品！\n";
            }

        }

        private void textBox2_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!(char.IsNumber(e.KeyChar)) && e.KeyChar != (char)8)       //只能输入数字
            {
                e.Handled = true;
            }
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            richTextBox1.SelectionStart = richTextBox1.Text.Length;
            richTextBox1.SelectionLength = 0;
            richTextBox1.Focus();               //使进度条一直在数据显示的最前端
        }

        private void Set_User(String User_Mode)
        {
            Play_Tips(User_Mode);
            label4.Text = User_Mode;
            label2.Text = "";
            pictureBox4.Visible = false;
            pictureBox5.Visible = true;     //重置条码框
            button3.BackgroundImage = Image.FromFile(@"E:\毕业论文\Something\Project\SuperMarket\" + User_Mode + ".png");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            User = !User;   //切换用户（true:Manager；false:Customer）

            if (User)
                Set_User("Manager");
            else
                Set_User("Customer");
        }
    }
}
