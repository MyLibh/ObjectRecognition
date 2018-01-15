using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;

using System.Net.Sockets;
using System.Net;

namespace Client
{
    class Client
    {
        private TcpClient client_ = new TcpClient();

        public Client()
        {
            client_.Connect(IPAddress.Parse("127.0.0.1"), 1234);
        }

        public bool sendTask(string file)
        {
 
            BinaryReader task = new BinaryReader(File.Open(file, FileMode.Open));
            StreamWriter sWriter = new StreamWriter(client_.GetStream());
            while(task.PeekChar() > -1)
            {
                char[] buf = task.ReadChars(1024);
                sWriter.Write(buf);
            }
            sWriter.Write("TASKEND");

            return true;
        }

        public bool getResult(string file)
        {

            BinaryReader task = new BinaryReader(File.Open(file, FileMode.Open));
            StreamWriter sWriter = new StreamWriter(client_.GetStream());
            while (task.PeekChar() > -1)
            {
                char[] buf = task.ReadChars(1024);
                sWriter.Write(buf);
            }
            sWriter.Write("TASKEND");

            return true;
        }
    }
}
