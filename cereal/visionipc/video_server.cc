#include<thread>
void run_video(std::string file_name)
{
    std::string path = "/data/openpilot";
    std::string full_path = path+file_name;
    cv::VideoCapture cap(full_path);
    if(!cap.isOpened())
    {
        std::cerr<<"can not open video"<<std::endl;
        return;
    }
    cv::Mat frame;
    size_t rgb_width = static_cast<size_t>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    size_t rgb_height = static_cast<size_t>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    VisionIpcServer vipc_server(file_name);
    vipc_server.create_buffers_from_video(VISION_STREAM_RGB_BACK,4,cap);
    vipc_server.start_listener();
    while(cap.read(frame))
    {
        VisionBuf* buf = vipc_server.get_buffer(VISION_STREAM_RGB_BACK);
        std::memcpy(buf->addr,frame.data,frame.total()*frame.elemSize());
        VisionIpcBufExtra struct VisionIpcBufExtra extra;
        extra.timestamp_eof=static_cast<uint64_t>(2.5368465);
        vipc_server.send(buf,&extra,false);
    }

}

imt main(){
    std::vector<std::thread> threads;
    for(int i = 1;i<=1;i++)
    {
        std::string file_name;
        file_name = "video"+std::to_string(i)+".mp4";
        threads.emplace_back(run_video,file_name);
    }
    for(auto &t : threads) t.join();
}