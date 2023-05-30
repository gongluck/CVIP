-- upload.lua
-- ==========================================
local upload = require "resty.upload"
local cjson = require "cjson"
local chunk_size = 4096
local form, err = upload:new(chunk_size)

if not form then
    ngx.exit(ngx.HTTP_INTERNAL_SERVER_ERROR)
end
form:set_timeout(1000)

-- 字符串 split 分割
string.split = function(s, p)
    local rt = {}
    string.gsub(s, '[^' .. p .. ']+', function(w)
        table.insert(rt, w)
    end)
    return rt
end

-- 支持字符串前后 trim
string.trim = function(s)
    return (s:gsub("^%s*(.-)%s*$", "%1"))
end

local function get_file_name(header_value)
    local kvlist = string.split(header_value, ';')
    for _, kv in ipairs(kvlist) do
        local seg = string.trim(kv)
        if seg:find("filename") then
            local kvfile = string.split(seg, "=")
            local filename = string.sub(kvfile[2], 2, -2)
            return filename
        end
    end
end

-- 文件保存的根路径
local saveRootPath = "/tmp/test/";
-- 保存的文件对象
local fileToSave
-- 文件是否成功保存
local ret_save = false
while true do
    local typ, res, err = form:read()
    if not typ then
        ngx.say("failed to read: ", err)
        return
    end
    if typ == "header" then
        -- 开始读取header
        local key = res[1]
        local value = res[2]
        if key == "Content-Disposition" then
            -- 解析出本次上传的文件名
            -- form-data; name="testFileName"; filename="testfile.txt"
            local filename = get_file_name(value)
            if filename then
                local fullpath = saveRootPath .. filename
                if io.open(fullpath) then
                    ngx.say("文件已存在")
                    break
                end

                fileToSave = io.open(fullpath, "w+")
                if not fileToSave then
                    ngx.say("文件打开失败 ", filename)
                    break
                end
            end
        end
    elseif typ == "body" then
        -- 开始读取 http body
        if fileToSave then
            fileToSave:write(res)
        end
    elseif typ == "part_end" then
        -- 文件写结束，关闭文件
        if fileToSave then
            fileToSave:close()
            fileToSave = nil
        end

        ret_save = true
    elseif typ == "eof" then
        -- 文件读取结束
        break
    else
        ngx.log(ngx.INFO, "do other things")
    end
end

if ret_save then
    ngx.say("上传文件成功。")
else
    ngx.say("上传文件失败。")
end
