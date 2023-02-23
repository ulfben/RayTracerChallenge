#pragma once
#include "pch.h"
#include "Tuple.h"
#include "StringHelpers.h"
#include "WorkQue.h"
//A neat API example by lippuu: https://gist.github.com/lippuu/cbf4fa62fe8eed408159a558ff5c96ee
static constexpr uint16_t CHANNELS = 3; //RGB
static constexpr uint16_t CHARS_PER_CHANNEL = 4; //"255 "
static constexpr uint16_t CHARS_PER_PIXEL = CHANNELS * CHARS_PER_CHANNEL; //"255 255 255 ".size() == 12
static constexpr uint16_t MAX_PIXELS_PER_LINE = (PPM_MAX_LINE_LENGTH / CHARS_PER_PIXEL); //(70/12) == 5
static constexpr uint16_t SPACES_PER_PIXEL = 3; //"255 255 255 "
static constexpr uint16_t MAX_SPACES_PER_LINE = MAX_PIXELS_PER_LINE * SPACES_PER_PIXEL; //5*3 == 15. 15*MAX_CHARACTERS_PER_PIXEL = 60 (a safe value <70) 
constexpr void ppm_add_linebreaks(std::string& str) noexcept;

std::string ppm_header(size_t width, size_t height) {
    #pragma warning( suppress : 26481 ) //spurious warning; "don't use pointer arithmetic" 
    return std::format("{}\n{} {}\n{}\n"sv, PPM_VERSION, width, height, PPM_MAX_BYTE_VALUE);
}  

std::string to_ppm_parallelized(std::span<const Color> bitmap, size_t width, size_t height) {
    std::vector<ByteColor> buffer(bitmap.begin(), bitmap.end()); //bulk-convert pixels from 0.0f-1.0f to 0-255
    WorkQue worker;    
    std::vector<std::string> out(worker.thread_count());
    std::ranges::for_each(out, [partition_size = worker.partition_size()](auto& out_part) {
        out_part.reserve(partition_size*CHARS_PER_PIXEL); }
    );
    out[0].append(ppm_header(width, height));
    worker.schedule(buffer.size(), [&out, &buffer](size_t part, size_t i) noexcept {                            
            out[part].append(to_string_with_trailing_space(buffer[i]));            
    });  
    worker.run_in_parallel();  
    buffer.clear();         
    worker.clear();        
    return std::accumulate(out.begin()+1, out.end(), out[0]);
}

class Canvas final {
public:
    using size_type = size_t;
    using value_type = Color;        
    using container = std::vector<value_type>;
    using reference = container::reference;
    using const_reference = container::const_reference;
    using pointer = container::pointer;
    using const_pointer = container::const_pointer;
    using iterator = container::iterator;
    using const_iterator = container::const_iterator;

    constexpr Canvas(size_type width, size_type height) {
        resize(width, height);
        clear();
    }

    constexpr void resize(size_type w, size_type h) {
        assert(w > 0 && h > 0 && "Canvas dimensions must be non-zero.");
        bitmap.resize(w * h);
        _width = w;
        _height = h;
    }

    constexpr void clear(const value_type& col = value_type{ .0f, .0f, .0f }) noexcept {
        std::ranges::fill(bitmap, col);
    }       
    constexpr void set(size_type i, const value_type& col) noexcept {
        if (i < bitmap.size()) {            
            bitmap[i] = col;
        }
    }
    constexpr void set(size_type x, size_type y, const value_type& col) noexcept {
        if (x < _width && y < _height) {
            const auto row = y * _width;
            bitmap[row + x] = col;
        }
    }
    constexpr void set(const Point& p, const value_type& col) noexcept {
        constexpr auto MAX = narrow_cast<size_t>(std::numeric_limits<Real>::max());            
        if (p.x >= 0 && p.y >= 0 && p.x < float(MAX) && p.y < float(MAX)) {
            set(narrow_cast<size_type>(p.x), narrow_cast<size_type>(p.y), col);
        }
    }
    constexpr const_reference get(size_type x, size_type y) const noexcept {
        assert(x <= _width && "Canvas::get called with invalid x position");
        assert(y <= _height && "Canvas::get called with invalid y position");
        const auto row = y *_width;
        return bitmap[row + x];
    }
    constexpr const_reference operator[](size_type i) const noexcept {
        assert(size() < i); 
        return bitmap[i];
    }
    constexpr reference operator[](size_type i) noexcept {        
        assert(size() < i); 
        return bitmap[i];
    }
    constexpr size_type width() const noexcept {
        return _width;
    }
    constexpr size_type height() const noexcept {
        return _height;
    }
    constexpr Real widthf() const noexcept {
        return static_cast<Real>(_width);
    }
    constexpr Real heightf() const noexcept {
        return static_cast<Real>(_height);
    }    
    constexpr const_pointer data() const noexcept { return bitmap.data(); }
    constexpr const_iterator begin() const noexcept { return bitmap.begin(); }
    constexpr const_iterator end() const noexcept { return bitmap.end(); }
    constexpr size_type size() const noexcept { return bitmap.size(); }

    std::string to_ppm() const {
        std::string ppm = to_ppm_parallelized(bitmap, _width, _height);
        if (width() < MAX_PIXELS_PER_LINE) {
            ppm_add_linebreaks(ppm); //TODO: simplified linebreak when we can fit entire width() into the output file. 
        } else {
            ppm_add_linebreaks(ppm);
        }
        return ppm;        
    }       
    
private:
    container bitmap;
    size_type _width = 0;
    size_type _height = 0;
};

constexpr void ppm_add_linebreaks(std::string& str) noexcept{        
    using size_type = std::string::size_type;    
    const size_type header_end = str.find_last_of("\n"sv);                    
    const size_type end = str.size();
    size_type space_count = 0;
    for(size_type i = header_end; i < end; ++i) {
        if(str[i] != ' ') { continue; }                
        if (++space_count == MAX_SPACES_PER_LINE) {
             str.replace(i, 1, "\n"sv);
             space_count = 0;
        }                
    }
    if (str.back() == ' ') { str.pop_back(); }
    str.append("\n"sv);    
}

void save_to_file(const Canvas& img, std::string_view path) {
    std::ofstream ofs(path.data(), std::ofstream::out);    
    ofs << img.to_ppm();
}
