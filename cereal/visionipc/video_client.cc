

void recv_frame(std::string file_name,std::string file_name)
{
    pthread_mutex_t mutex;
    VisionIpcClient client = VisionIpcClient(file_name,VISION_STREAM_RGB_BACK);
    while(!client.connect(false))
    {
        util:sleep_for(100);
    }
    while(true)
    {
        if(client.connected)
        {
            size_t width = 1920;
            size_t height = 1080;
            size_t channel = 3;
            size_t imageDataSize = width*height*channel;
            std::vector<uint8_t> imageDataSave(imageDataSize);
            cv::namedWindow(frame_name,cv::WINDOW_NORMAL);
            VisionIpcBufExtra extra_recv={0};
            VisionBuf* recv_buf = client.recv(&extra_recv,300);
            if(recv_buf)
            {
                pthread_mutex_lock(&mutex);
                cv::Mat trans_frame(height,width,CV_8UC3,recv_buf->addr);
                cv::resize(trans_frame,trans_frame,cv::Size(300,300));
                pthread_mutex_unlock(&mutex);
                cv::imshow(file_name,trans_frame);
                char key = cv::waitKey(1);
                if(key =='q')
                {
                    break;
                }
            }
            
        }
    }
    cv::destroyAllWindows();
}

int main(){
    std::vector<std::thread> threads;
    for(int i = 1;i<=1;i++)
    {
        std::string file_name,frame_name;
        file_name = "video"+std::to_string(i)+".mp4";
        frame_name = "video" +std::to_string(i);
        thread::emplace_back(recv_frame,file_name,frame_name);
    }
    for(auto &t:threads) t.join();
}