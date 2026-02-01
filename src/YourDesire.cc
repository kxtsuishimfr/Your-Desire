-- ─────────────── ✦ Rivals - Your Desire ✦ ───────────────
--  Created by: primesto.fx
--  Maintained by: primesto.fx
--  DM on Discord for requests: primesto.fx
-- ────────────────────────────────────────────────────────────────

local Players = game:GetService("Players")
local TweenService = game:GetService("TweenService")
local HttpService = game:GetService("HttpService")
local Workspace = game:GetService("Workspace")
local Lighting = game:GetService("Lighting")
local RunService = game:GetService("RunService")
local UserInputService = game:GetService("UserInputService")
local ReplicatedStorage = game:GetService("ReplicatedStorage")

---------------------------------------------------------------------------

-- ** Interactables API ** --

local ToggleAPI = setmetatable({}, { __mode = "k" })
local DropdownAPI = setmetatable({}, { __mode = "k" })
local KeybindAPI = setmetatable({}, { __mode = "k" })
local SliderAPI = setmetatable({}, { __mode = "k" })
local ButtonAPI = setmetatable({}, { __mode = "k" })
local ColorPickerAPI = setmetatable({}, { __mode = "k" })
local NotificationAPI 

--------------------------------------------------------------------------

-- ** Notification tracking ** --

local RECENT_NOTIFS = setmetatable({}, { __mode = "k" })

---------------------------------------------------------------------------

-- ** Color palette (Screen_GUI) **
local COLORS = {
    bg = Color3.fromRGB(20,18,30),     
    panel = Color3.fromRGB(28,24,38),       
    panelAlt = Color3.fromRGB(38,30,50),    
    panelDark = Color3.fromRGB(18,16,25),   
    divider = Color3.fromRGB(70,50,80),    
    accent = Color3.fromRGB(200,80,180),    
    accentHover = Color3.fromRGB(220,100,200), 
    text = Color3.fromRGB(240,240,245),     
    textDim = Color3.fromRGB(180,170,190),  
    tabText = Color3.fromRGB(220,200,230),  
    highlight = Color3.fromRGB(90,70,100),  
    white = Color3.fromRGB(255,255,255),
    close = Color3.fromRGB(255,200,200),   
    closeHover = Color3.fromRGB(255,120,150), 
}


-------------------------------------------------------

-- ** Theme System ** --

local function shallowCopy(t)
    local o = {}
    for k,v in pairs(t) do o[k] = v end
    return o
end

local LAST_THEME = nil

local THEMES = {
    ["Your Desire"] = shallowCopy(COLORS),
    ["Gilded Crown"] = {
        bg = Color3.fromRGB(18,16,20), panel = Color3.fromRGB(34,28,24), panelAlt = Color3.fromRGB(46,40,36),
        panelDark = Color3.fromRGB(14,12,10), divider = Color3.fromRGB(120,90,50), accent = Color3.fromRGB(220,180,80),
        accentHover = Color3.fromRGB(240,200,120), text = Color3.fromRGB(250,245,235), textDim = Color3.fromRGB(190,170,150),
        tabText = Color3.fromRGB(230,210,190), highlight = Color3.fromRGB(100,90,70), white = Color3.fromRGB(255,255,255),
        close = Color3.fromRGB(255,220,200), closeHover = Color3.fromRGB(255,160,120),
    },
    ["Blue Hour"] = {
        bg = Color3.fromRGB(12,18,30), panel = Color3.fromRGB(20,28,42), panelAlt = Color3.fromRGB(28,36,52),
        panelDark = Color3.fromRGB(8,10,16), divider = Color3.fromRGB(60,80,110), accent = Color3.fromRGB(80,160,220),
        accentHover = Color3.fromRGB(110,190,240), text = Color3.fromRGB(235,245,255), textDim = Color3.fromRGB(170,190,210),
        tabText = Color3.fromRGB(200,220,240), highlight = Color3.fromRGB(40,60,80), white = Color3.fromRGB(255,255,255),
        close = Color3.fromRGB(255,200,200), closeHover = Color3.fromRGB(255,120,150),
    },
    ["Verdant Pulse"] = {
        bg = Color3.fromRGB(14,28,20), panel = Color3.fromRGB(24,44,34), panelAlt = Color3.fromRGB(32,56,42),
        panelDark = Color3.fromRGB(10,18,12), divider = Color3.fromRGB(50,100,70), accent = Color3.fromRGB(80,200,120),
        accentHover = Color3.fromRGB(110,230,150), text = Color3.fromRGB(235,250,240), textDim = Color3.fromRGB(170,200,180),
        tabText = Color3.fromRGB(200,230,210), highlight = Color3.fromRGB(40,70,50), white = Color3.fromRGB(255,255,255),
        close = Color3.fromRGB(255,200,200), closeHover = Color3.fromRGB(255,120,150),
    },
}


-- ** Apply Theme ** --

local function ApplyTheme(name)
    local prev = shallowCopy(COLORS)
    LAST_THEME = prev
    local theme = (type(name) == "string" and THEMES[name]) and THEMES[name] or THEMES["Your Desire"]
    COLORS = shallowCopy(theme)

    local map = {}
    for k,v in pairs(prev) do if COLORS[k] then map[v] = COLORS[k] end end

    local function safeLerp(a,b,t)
        if typeof(a) ~= "Color3" or typeof(b) ~= "Color3" then return nil end
        return a:Lerp(b, t)
    end
    pcall(function()
        local pv, nv = prev, COLORS
        local a1 = safeLerp(pv.panel or pv.bg, pv.text, 0.18)
        local b1 = safeLerp(nv.panel or nv.bg, nv.text, 0.18)
        if a1 and b1 then map[a1] = b1 end
        local a2 = safeLerp(pv.accent or pv.text, pv.white or Color3.new(1,1,1), 0.18)
        local b2 = safeLerp(nv.accent or nv.text, nv.white or Color3.new(1,1,1), 0.18)
        if a2 and b2 then map[a2] = b2 end
        local a3 = safeLerp(pv.panel or pv.bg, pv.text, 0.14)
        local b3 = safeLerp(nv.panel or nv.bg, nv.text, 0.14)
        if a3 and b3 then map[a3] = b3 end
        for i=1,3 do
            local oldSurf = (pv.bg or pv.panel or pv.panelAlt)
            local newSurf = (nv.bg or nv.panel or nv.panelAlt)
            if oldSurf and newSurf and a2 and b2 then
                local oldT = safeLerp(oldSurf, a2, 0.06)
                local newT = safeLerp(newSurf, b2, 0.06)
                if oldT and newT then map[oldT] = newT end
            end
        end
    end)

    local function colorDist(a,b)
        local dr = a.r - b.r
        local dg = a.g - b.g
        local db = a.b - b.b
        return dr*dr + dg*dg + db*db
    end

    local function findMapped(col)
        if not col or typeof(col) ~= "Color3" then return nil end
        for old,new in pairs(map) do if old == col then return new end end
        local best, bestd = nil, 1e9
        for old,new in pairs(map) do
            local d = colorDist(old, col)
            if d < bestd then bestd = d; best = new end
        end
        if best and bestd < 0.006 then 
            return best
        end
        return nil
    end

    if gui and gui.Parent then
        pcall(function()
            for _,obj in ipairs(gui:GetDescendants()) do
                if obj:IsA("GuiObject") then
                    pcall(function()
                        local ok, bg = pcall(function() return obj.BackgroundColor3 end)
                        if ok and typeof(bg) == "Color3" then
                            local m = findMapped(bg)
                            if m then obj.BackgroundColor3 = m end
                        end
                    end)
                    pcall(function()
                        local ok2, tx = pcall(function() return obj.TextColor3 end)
                        if ok2 and typeof(tx) == "Color3" then
                            local m2 = findMapped(tx)
                            if m2 then obj.TextColor3 = m2 end
                        end
                    end)
                end
                if obj:IsA("UIStroke") then
                    pcall(function()
                        local ok3, c = pcall(function() return obj.Color end)
                        if ok3 and typeof(c) == "Color3" then
                            local m3 = findMapped(c)
                            if m3 then obj.Color = m3 end
                        end
                    end)
                end
            end
            if root and root:IsA("GuiObject") then root.BackgroundColor3 = COLORS.bg end
            if tabsUnderlay and tabsUnderlay:IsA("GuiObject") then tabsUnderlay.BackgroundColor3 = COLORS.panel end
            if tabsBar and tabsBar:IsA("GuiObject") then
                for _,c in ipairs(tabsBar:GetChildren()) do
                    if c:IsA("TextButton") then c.BackgroundColor3 = COLORS.bg; c.TextColor3 = COLORS.tabText end
                end
            end
            if closeBtn and closeBtn:IsA("GuiObject") then closeBtn.TextColor3 = COLORS.close end
        end)
    end

    pcall(function()
        for k,api in pairs(ToggleAPI) do
            if type(api) == "table" and type(api.Get) == "function" and type(api.Set) == "function" then
                local prevOn = api.OnToggle
                api.OnToggle = nil
                pcall(api.Set, api.Get())
                api.OnToggle = prevOn
            end
        end
        for k,api in pairs(SliderAPI) do
            if type(api) == "table" and type(api.Get) == "function" and type(api.Set) == "function" then
                local prevOn = api.OnChange
                api.OnChange = nil
                pcall(api.Set, api.Get())
                api.OnChange = prevOn
            end
        end
        for k,api in pairs(ColorPickerAPI) do
            if type(api) == "table" and type(api.Get) == "function" and type(api.Set) == "function" then
                local prevOn = api.OnChange
                api.OnChange = nil
                pcall(api.Set, api.Get())
                api.OnChange = prevOn
            end
        end
        for k,api in pairs(DropdownAPI) do
            if type(api) == "table" and type(api.Get) == "function" and type(api.Set) == "function" then
                local sel = api.Get()
                if type(sel) == "table" and sel.index then pcall(api.Set, sel.index) end
            end
        end
    end)
end

-- ** Themed Registry ** --

local THEME_REGISTRY = {}

local function snapshotColors(obj)
    local t = {}
    pcall(function()
        if obj:IsA("GuiObject") then
            if obj.BackgroundColor3 ~= nil then t.bg = obj.BackgroundColor3 end
            if obj.TextColor3 ~= nil then t.text = obj.TextColor3 end
        end
        for _,c in ipairs(obj:GetChildren()) do
            if c:IsA("UIStroke") then
                t.stroke = t.stroke or {}
                table.insert(t.stroke, c.Color)
            end
        end
    end)
    return t
end

local function RegisterThemed(obj, refreshFn)
    if not obj or typeof(obj) ~= "Instance" then return end
    local entry = { obj = obj, snapshot = snapshotColors(obj), refresh = (type(refreshFn) == "function") and refreshFn or nil }
    table.insert(THEME_REGISTRY, entry)
    return entry
end

local function RefreshRegisteredThemed()
    if #THEME_REGISTRY == 0 then return end
    pcall(function()
        local prev = LAST_THEME or {}
        local cur = COLORS or {}
        local map = {}
        for k,v in pairs(prev) do if cur[k] then map[v] = cur[k] end end

        local function safeLerp(a,b,t)
            if typeof(a) ~= "Color3" or typeof(b) ~= "Color3" then return nil end
            return a:Lerp(b, t)
        end
        pcall(function()
            local a1 = safeLerp(prev.panel or prev.bg, prev.text, 0.18)
            local b1 = safeLerp(cur.panel or cur.bg, cur.text, 0.18)
            if a1 and b1 then map[a1] = b1 end
            local a2 = safeLerp(prev.accent or prev.text, prev.white or Color3.new(1,1,1), 0.18)
            local b2 = safeLerp(cur.accent or cur.text, cur.white or Color3.new(1,1,1), 0.18)
            if a2 and b2 then map[a2] = b2 end
            local a3 = safeLerp(prev.panel or prev.bg, prev.text, 0.14)
            local b3 = safeLerp(cur.panel or cur.bg, cur.text, 0.14)
            if a3 and b3 then map[a3] = b3 end
            for i=1,3 do
                local oldSurf = (prev.bg or prev.panel or prev.panelAlt)
                local newSurf = (cur.bg or cur.panel or cur.panelAlt)
                if oldSurf and newSurf and a2 and b2 then
                    local oldT = safeLerp(oldSurf, a2, 0.06)
                    local newT = safeLerp(newSurf, b2, 0.06)
                    if oldT and newT then map[oldT] = newT end
                end
            end
        end)

        local function colorDist(a,b)
            local dr = a.r - b.r
            local dg = a.g - b.g
            local db = a.b - b.b
            return dr*dr + dg*dg + db*db
        end

        local function findMapped(col)
            if not col or typeof(col) ~= "Color3" then return nil end
            for old,new in pairs(map) do if old == col then return new end end
            local best, bestd = nil, 1e9
            for old,new in pairs(map) do
                local d = colorDist(old, col)
                if d < bestd then bestd = d; best = new end
            end
            if best and bestd < 0.006 then return best end
            local nearest, nd = nil, 1e9
            for k,v in pairs(cur) do
                local d = colorDist(v, col)
                if d < nd then nd = d; nearest = v end
            end
            if nearest then return nearest end
            return nil
        end

        for _,e in ipairs(THEME_REGISTRY) do
            local o = e.obj
            local s = e.snapshot
            if o and o.Parent then
                pcall(function()
                    if s.bg and pcall(function() return o.BackgroundColor3 end) then
                        local m = findMapped(s.bg)
                        if m then o.BackgroundColor3 = m end
                    end
                    if s.text and pcall(function() return o.TextColor3 end) then
                        local m = findMapped(s.text)
                        if m then o.TextColor3 = m end
                    end
                    if pcall(function() return o.ImageColor3 end) then
                        local ok, curVal = pcall(function() return o.ImageColor3 end)
                        if ok and typeof(curVal) == "Color3" then
                            local m = findMapped(curVal)
                            if m then o.ImageColor3 = m end
                        end
                    end
                    if pcall(function() return o.BorderColor3 end) then
                        local ok2, curVal2 = pcall(function() return o.BorderColor3 end)
                        if ok2 and typeof(curVal2) == "Color3" then
                            local m2 = findMapped(curVal2)
                            if m2 then o.BorderColor3 = m2 end
                        end
                    end
                end)

                if s.stroke and #s.stroke > 0 then
                    local strokes = {}
                    for _,c in ipairs(o:GetChildren()) do if c:IsA("UIStroke") then table.insert(strokes, c) end end
                    for i,old in ipairs(s.stroke) do
                        local target = strokes[i]
                        if target and typeof(old) == "Color3" then
                            local m = findMapped(old)
                            if m then pcall(function() target.Color = m end) end
                        end
                    end
                end
                if type(e.refresh) == "function" then
                    pcall(e.refresh)
                end
            end
        end
    end)
end

do
    local _orig = ApplyTheme
    ApplyTheme = function(name)
        _orig(name)
        pcall(RefreshRegisteredThemed)
        pcall(function()
            for k,api in pairs(ToggleAPI) do if type(api) == "table" and api.Set and api.Get then local on = api.OnToggle; api.OnToggle = nil; pcall(api.Set, api.Get()); api.OnToggle = on end end
            for k,api in pairs(SliderAPI) do if type(api) == "table" and api.Set and api.Get then local on = api.OnChange; api.OnChange = nil; pcall(api.Set, api.Get()); api.OnChange = on end end
            for k,api in pairs(ColorPickerAPI) do if type(api) == "table" and api.Set and api.Get then local on = api.OnChange; api.OnChange = nil; pcall(api.Set, api.Get()); api.OnChange = on end end
        end)
    end
end


-----------------------------------------------------------------------------
local player = Players.LocalPlayer
local FIRST_TAB = nil -- ** select first tab
local gui = Instance.new("ScreenGui")
gui.Name = "SCREEN_GUI"
gui.ResetOnSpawn = false


local ok = pcall(function() gui.Parent = game:GetService("CoreGui") end)
if not ok then
    if player then
        gui.Parent = player:WaitForChild("PlayerGui")
    else
        gui.Parent = game:GetService("CoreGui")
    end
end

-------------------------------------------------------------------------------

-- ** makeTab

local function makeTab(name, tabsParent, pagesParent, onSelect, colHeaders)
    local btn = Instance.new("TextButton")
    local corner = Instance.new("UICorner") corner.CornerRadius = UDim.new(0, 6) corner.Parent = btn
    btn.Name = name .. "Tab"
    btn.Size = UDim2.new(1, -12, 0, 32)
    btn.AutoButtonColor = false
    btn.Font = Enum.Font.GothamBold
    btn.TextSize = 16
    btn.Text = name
    btn.BackgroundColor3 = COLORS.panel
    btn.TextColor3 = COLORS.tabText
    btn.BorderSizePixel = 0
    btn.TextXAlignment = Enum.TextXAlignment.Left
    local btnPad = Instance.new("UIPadding") btnPad.Parent = btn; btnPad.PaddingLeft = UDim.new(0, 12)
    btn.ZIndex = 10

    -- ** active tab indicator 
    local indicator = Instance.new("Frame")
    indicator.Name = "ActiveIndicator"
    indicator.Size = UDim2.new(0, 4, 1, -8)
    indicator.Position = UDim2.new(1, -6, 0, 4)
    indicator.BackgroundColor3 = COLORS.accent
    indicator.BackgroundTransparency = 1
    indicator.ZIndex = btn.ZIndex - 1
    local indCorner = Instance.new("UICorner") indCorner.CornerRadius = UDim.new(0, 2) indCorner.Parent = indicator
    indicator.Parent = btn

    local page = Instance.new("Frame")
    page.Name = name .. "Page"
    page.Size = UDim2.new(1,0,1,0)
    page.BackgroundTransparency = 1
    local pageLayout = Instance.new("UIListLayout") pageLayout.Parent = page
    pageLayout.SortOrder = Enum.SortOrder.LayoutOrder
    pageLayout.Padding = UDim.new(0, 0)
    local pagePad = Instance.new("UIPadding") pagePad.Parent = page
    pagePad.PaddingLeft = UDim.new(0, 8)
    pagePad.PaddingRight = UDim.new(0, 8)
    pagePad.PaddingTop = UDim.new(0, 8)
    pagePad.PaddingBottom = UDim.new(0, 8)

    if tabsParent then btn.Parent = tabsParent end
    RegisterThemed(btn, function()
        pcall(function()
            local ind = btn:FindFirstChild("ActiveIndicator")
            local isActive = ind and ind.BackgroundTransparency == 0
            if isActive then
                btn.TextColor3 = COLORS.white
                btn.BackgroundColor3 = COLORS.accent
                if ind then ind.BackgroundColor3 = COLORS.accent end
            else
                btn.TextColor3 = COLORS.tabText
                btn.BackgroundColor3 = COLORS.panel
                if ind then ind.BackgroundColor3 = COLORS.accent end
                if ind then ind.BackgroundTransparency = 1 end
            end
        end)
    end)
    if pagesParent then page.Parent = pagesParent end
    RegisterThemed(page, function()
        pcall(function()
            if page and page:IsA("GuiObject") then
                page.BackgroundTransparency = 1
            end
        end)
    end)




    btn.MouseButton1Click:Connect(function()
        if type(onSelect) == "function" then pcall(onSelect, btn, page) end
    end)

    -- ** Hover thing
    btn.MouseEnter:Connect(function()
        local isActive = (btn.TextColor3 == COLORS.white)
        local target = isActive and COLORS.accentHover or COLORS.panelAlt
        pcall(function() TweenService:Create(btn, TweenInfo.new(0.12), {BackgroundColor3 = target}):Play() end)
    end)
    btn.MouseLeave:Connect(function()
        local isActive = (btn.TextColor3 == COLORS.white)
        local target = isActive and COLORS.accent or COLORS.panel
        pcall(function() TweenService:Create(btn, TweenInfo.new(0.12), {BackgroundColor3 = target}):Play() end)
    end)

---------------------------------------------------------------------------------

-- ** Col Stuff

    -- ** Left col (top)
    local leftCol = Instance.new("Frame")
    leftCol.Name = "LeftCol"
    leftCol.Size = UDim2.new(1, 0, 0, 0)
    leftCol.BackgroundTransparency = 1
    leftCol.Parent = page
    leftCol.AutomaticSize = Enum.AutomaticSize.Y
    leftCol.LayoutOrder = 0
    RegisterThemed(leftCol)
    leftCol.ClipsDescendants = false
    local list = Instance.new("UIListLayout") list.Parent = leftCol
    list.SortOrder = Enum.SortOrder.LayoutOrder
    list.Padding = UDim.new(0, 8)
    local leftPad = Instance.new("UIPadding") leftPad.Parent = leftCol
    leftPad.PaddingLeft = UDim.new(0, 10)
    leftPad.PaddingRight = UDim.new(0, 10)
    leftPad.PaddingTop = UDim.new(0, 8)
    leftPad.PaddingBottom = UDim.new(0, 8)

    -- ** Header for left col
    if colHeaders and colHeaders.Left then
        local hdr = Instance.new("TextLabel")
        hdr.Name = "Header"
        hdr.Size = UDim2.new(1, 0, 0, 20)
        hdr.BackgroundTransparency = 1
        hdr.Font = Enum.Font.GothamBold
        hdr.TextSize = 14
        hdr.Text = tostring(colHeaders.Left)
        hdr.TextColor3 = COLORS.accent
        hdr.TextXAlignment = Enum.TextXAlignment.Left
        hdr.LayoutOrder = 0
        hdr.Parent = leftCol
    end


    -- ** Right col (bottom)
    local rightCol = Instance.new("Frame")
    rightCol.Name = "RightCol"
    rightCol.Size = UDim2.new(1, 0, 0, 0)
    rightCol.BackgroundTransparency = 1
    rightCol.Parent = page
    rightCol.AutomaticSize = Enum.AutomaticSize.Y
    rightCol.LayoutOrder = 2
    RegisterThemed(rightCol)
    rightCol.ClipsDescendants = false
    local list2 = Instance.new("UIListLayout") list2.Parent = rightCol
    list2.SortOrder = Enum.SortOrder.LayoutOrder
    list2.Padding = UDim.new(0, 8)
    local rightPad = Instance.new("UIPadding") rightPad.Parent = rightCol
    rightPad.PaddingLeft = UDim.new(0, 10)
    rightPad.PaddingRight = UDim.new(0, 10)
    rightPad.PaddingTop = UDim.new(0, 8)
    rightPad.PaddingBottom = UDim.new(0, 8)

    -- ** Header for right col
    if colHeaders and colHeaders.Right then
        local hdrr = Instance.new("TextLabel")
        hdrr.Name = "Header"
        hdrr.Size = UDim2.new(1, 0, 0, 20)
        hdrr.BackgroundTransparency = 1
        hdrr.Font = Enum.Font.GothamBold
        hdrr.TextSize = 14
        hdrr.Text = tostring(colHeaders.Right)
        hdrr.TextColor3 = COLORS.accent
        hdrr.TextXAlignment = Enum.TextXAlignment.Left
        hdrr.LayoutOrder = 1
        hdrr.Parent = rightCol
    end

    -- ** Horizontal divider between cols
    local divider = Instance.new("Frame")
    divider.Name = "Divider"
    divider.Size = UDim2.new(1, 0, 0, 1)
    divider.BackgroundColor3 = COLORS.divider
    divider.Parent = page
    divider.BorderSizePixel = 0
    divider.LayoutOrder = 1
    RegisterThemed(divider)
    divider.AnchorPoint = Vector2.new(0, 0.5)
    RegisterThemed(divider)

    local tab = {
        button = btn,
        page = page,
        LeftCol = leftCol,
        RightCol = rightCol,
        MakeCol = function(colName, size, pos, headerText)
            local col = Instance.new("Frame")
            col.Name = colName or "Col"
            col.Size = size or UDim2.new(0.5, -12, 1, -12)
            col.Position = pos or UDim2.new(0.5, 8, 0, 8)
            col.BackgroundColor3 = COLORS.panel
            col.Parent = page
            col.ClipsDescendants = true
            local l = Instance.new("UIListLayout") l.Parent = col
            l.SortOrder = Enum.SortOrder.LayoutOrder
            if headerText then
                local h = Instance.new("TextLabel")
                h.Name = "Header"
                h.Size = UDim2.new(1, -12, 0, 24)
                h.Position = UDim2.new(0, 6, 0, 6)
                h.BackgroundTransparency = 1
                h.Font = Enum.Font.GothamBold
                h.TextSize = 16
                h.Text = tostring(headerText)
                h.TextColor3 = COLORS.textDim
                h.TextXAlignment = Enum.TextXAlignment.Left
                h.LayoutOrder = 0
                h.Parent = col
            end
            return col
        end,
    }

    pcall(function()
        if FIRST_TAB == nil then
            FIRST_TAB = { button = btn, page = page }
        end
    end)

    return tab
end
---------------------------------------------------------------------------

-- ** makeToggle

local function makeToggle(parent, labelText)
    local frame = Instance.new("Frame")
    frame.Size = UDim2.new(1, 0, 0, 36)
    frame.BackgroundTransparency = 1
    frame.Parent = parent

    local label = Instance.new("TextLabel")
    label.Size = UDim2.new(0.72, -6, 1, 0)
    label.BackgroundTransparency = 1
    label.Text = labelText or "Toggle"
    label.Font = Enum.Font.GothamBold
    label.TextSize = 17
    label.TextColor3 = COLORS.text
    label.TextXAlignment = Enum.TextXAlignment.Left
    label.Parent = frame
    RegisterThemed(label)

    local surfaceColor = COLORS.panel or COLORS.bg or COLORS.panelAlt
    local bgColor = COLORS.bg or COLORS.panel or surfaceColor
    local lightStroke = (COLORS.panel or COLORS.bg):Lerp(COLORS.text, 0.18)

    local toggle = Instance.new("Frame")
    toggle.Size = UDim2.new(0, 56, 0, 28)
    toggle.AnchorPoint = Vector2.new(1, 0.5)
    toggle.Position = UDim2.new(1, -8, 0.5, 0)
    toggle.BackgroundColor3 = surfaceColor
    toggle.ClipsDescendants = true
    toggle.Parent = frame
    RegisterThemed(toggle)

    local toggleCorner = Instance.new("UICorner")
    toggleCorner.CornerRadius = UDim.new(0, 14)
    toggleCorner.Parent = toggle

    local toggleStroke = Instance.new("UIStroke")
    toggleStroke.Thickness = 1
    toggleStroke.Color = lightStroke
    toggleStroke.Transparency = 0.85
    toggleStroke.Parent = toggle

    local fill = Instance.new("Frame")
    fill.Size = UDim2.new(0, 0, 1, 0)
    fill.Position = UDim2.new(0, 0, 0, 0)
    local accentVisible = (COLORS.accent or COLORS.text):Lerp(COLORS.white or Color3.new(1,1,1), 0.18)
    fill.BackgroundColor3 = accentVisible
    fill.BackgroundTransparency = 1
    fill.Parent = toggle
    RegisterThemed(fill)
    local fillCorner = Instance.new("UICorner")
    fillCorner.CornerRadius = UDim.new(0, 14)
    fillCorner.Parent = fill

    local knobShadow = Instance.new("Frame")
    knobShadow.Size = UDim2.new(0, 26, 0, 26)
    knobShadow.AnchorPoint = Vector2.new(0, 0.5)
    knobShadow.Position = UDim2.new(0, 4, 0.5, 0)
    knobShadow.BackgroundColor3 = (bgColor or surfaceColor):Lerp(COLORS.white or Color3.new(1,1,1), 0.9)
    knobShadow.BackgroundTransparency = 0.9
    knobShadow.ZIndex = 1
    knobShadow.Parent = toggle
    RegisterThemed(knobShadow)
    local ksCorner = Instance.new("UICorner")
    ksCorner.CornerRadius = UDim.new(0, 13)
    ksCorner.Parent = knobShadow

    local knob = Instance.new("Frame")
    knob.Size = UDim2.new(0, 22, 0, 22)
    knob.AnchorPoint = Vector2.new(0, 0.5)
    knob.Position = UDim2.new(0, 6, 0.5, 0)
    knob.BackgroundColor3 = COLORS.white or Color3.new(1,1,1)
    knob.ZIndex = 2
    knob.Parent = toggle
    RegisterThemed(knob)
    local kCorner = Instance.new("UICorner")
    kCorner.CornerRadius = UDim.new(0, 11)
    kCorner.Parent = knob

    local kStroke = Instance.new("UIStroke")
    kStroke.Thickness = 1
    kStroke.Color = (COLORS.panel or COLORS.bg):Lerp(COLORS.text, 0.14)
    kStroke.Transparency = 0.9
    kStroke.Parent = knob

    local inner = Instance.new("Frame")
    inner.Size = UDim2.new(0, 8, 0, 8)
    inner.AnchorPoint = Vector2.new(0.5, 0.5)
    inner.Position = UDim2.new(0.5, 0.5, 0.5, 0)
    inner.BackgroundColor3 = accentVisible
    inner.BackgroundTransparency = 1
    inner.ZIndex = 3
    inner.Parent = knob
    RegisterThemed(inner)
    local innerCorner = Instance.new("UICorner")
    innerCorner.CornerRadius = UDim.new(1, 0)
    innerCorner.Parent = inner

    RegisterThemed(frame, function()
        pcall(function()
            local api = ToggleAPI[frame]
            local curState = api and api.Get and api.Get() or state
            local surfaceColor = COLORS.panel or COLORS.bg or COLORS.panelAlt
            local bgColor = COLORS.bg or COLORS.panel or surfaceColor
            local lightStroke = (COLORS.panel or COLORS.bg):Lerp(COLORS.text, 0.18)
            local accentVisible = (COLORS.accent or COLORS.text):Lerp(COLORS.white or Color3.new(1,1,1), 0.18)
            if label then label.TextColor3 = COLORS.text end
            if fill then fill.BackgroundColor3 = accentVisible end
            if kStroke then kStroke.Color = (COLORS.panel or COLORS.bg):Lerp(COLORS.text, 0.14) end
            if knob then knob.BackgroundColor3 = COLORS.white or Color3.new(1,1,1) end
            if knobShadow then knobShadow.BackgroundColor3 = (bgColor or surfaceColor):Lerp(COLORS.white or Color3.new(1,1,1), 0.9) end
            if toggleStroke then toggleStroke.Color = curState and accentVisible or lightStroke end
            if curState then
                if fill then fill.Size = UDim2.new(1,0,1,0); fill.BackgroundTransparency = 0.45 end
                if knob then knob.Position = UDim2.new(1, -30, 0.5, 0) end
                if knobShadow then knobShadow.Position = UDim2.new(1, -34, 0.5, 0) end
                if inner then inner.Size = UDim2.new(0,10,0,10); inner.BackgroundTransparency = 0 end
            else
                if fill then fill.Size = UDim2.new(0,0,1,0); fill.BackgroundTransparency = 1 end
                if knob then knob.Position = UDim2.new(0,6,0.5,0) end
                if knobShadow then knobShadow.Position = UDim2.new(0,4,0.5,0) end
                if inner then inner.Size = UDim2.new(0,8,0,8); inner.BackgroundTransparency = 1 end
            end
        end)
    end)

    local state = false
    local tweenInfo = TweenInfo.new(0.18, Enum.EasingStyle.Quad, Enum.EasingDirection.Out)

    local function setVisual(on)
        state = not not on

        if state then
            TweenService:Create(fill, TweenInfo.new(0.22, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Size = UDim2.new(1, 0, 1, 0), BackgroundTransparency = 0.45}):Play()
            local targetBg = surfaceColor:Lerp(accentVisible, 0.06)
            TweenService:Create(toggle, TweenInfo.new(0.22, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {BackgroundColor3 = targetBg}):Play()
            TweenService:Create(knob, TweenInfo.new(0.26, Enum.EasingStyle.Back, Enum.EasingDirection.Out), {Position = UDim2.new(1, -30, 0.5, 0)}):Play()
            TweenService:Create(knobShadow, TweenInfo.new(0.26, Enum.EasingStyle.Back, Enum.EasingDirection.Out), {Position = UDim2.new(1, -34, 0.5, 0), BackgroundTransparency = 0.92}):Play()
            TweenService:Create(inner, TweenInfo.new(0.16, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Size = UDim2.new(0, 10, 0, 10), BackgroundTransparency = 0}):Play()
            toggleStroke.Color = accentVisible
            TweenService:Create(kStroke, TweenInfo.new(0.18), {Transparency = 1}):Play()
        else
            TweenService:Create(fill, TweenInfo.new(0.18, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Size = UDim2.new(0, 0, 1, 0), BackgroundTransparency = 1}):Play()
            TweenService:Create(toggle, tweenInfo, {BackgroundColor3 = surfaceColor}):Play()
            TweenService:Create(knob, TweenInfo.new(0.18, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Position = UDim2.new(0, 6, 0.5, 0)}):Play()
            TweenService:Create(knobShadow, TweenInfo.new(0.18, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Position = UDim2.new(0, 4, 0.5, 0), BackgroundTransparency = 0.9}):Play()
            TweenService:Create(inner, TweenInfo.new(0.14, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Size = UDim2.new(0, 8, 0, 8), BackgroundTransparency = 1}):Play()
            toggleStroke.Color = lightStroke
            TweenService:Create(kStroke, TweenInfo.new(0.18), {Transparency = 0.9}):Play()
        end

        local api = ToggleAPI[frame]
        if api and type(api.OnToggle) == "function" then
            pcall(api.OnToggle, state)
        end
    end

    ToggleAPI[frame] = {
        Set = function(v) setVisual(v) end,
        Get = function() return state end,
        OnToggle = nil,
    }

    toggle.MouseEnter:Connect(function()
        TweenService:Create(knob, TweenInfo.new(0.12, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Size = UDim2.new(0, 24, 0, 24)}):Play()
    end)
    toggle.MouseLeave:Connect(function()
        TweenService:Create(knob, TweenInfo.new(0.12, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Size = UDim2.new(0, 22, 0, 22)}):Play()
    end)

    toggle.Active = true
    toggle.InputBegan:Connect(function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            setVisual(not state)
            local s = (state and 1.03) or 0.97
            TweenService:Create(knob, TweenInfo.new(0.08, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Size = UDim2.new(0, 22 * s, 0, 22 * s)}):Play()
            delay(0.07, function()
                pcall(function()
                    TweenService:Create(knob, TweenInfo.new(0.12, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Size = UDim2.new(0, 22, 0, 22)}):Play()
                end)
            end)
        end
    end)

    local maxOrder = 0
    for _,c in ipairs(parent:GetChildren()) do
        if c ~= frame and (c:IsA("Frame") or c:IsA("TextLabel")) then
            maxOrder = math.max(maxOrder, c.LayoutOrder or 0)
        end
    end
    frame.LayoutOrder = maxOrder + 1

    setVisual(false)
    return frame
end

--------------------------------------------------------------------------

-- ** makeNotification
local function makeNotification(text, duration, parent, invoker)
    local dur = (type(duration) == "number" and duration > 0) and duration or 3
    local okCheck, allowed = pcall(function()
        if type(NotificationAPI) == "table" and type(NotificationAPI.CanCreate) == "function" then
            return NotificationAPI.CanCreate(invoker)
        end
        return true
    end)
    if not okCheck or not allowed then return nil end
    local parentGui
    do
        local Players = game:GetService("Players")
        local CoreGui = game:GetService("CoreGui")
        local lp = Players and Players.LocalPlayer
        if parent and parent ~= gui then
            parentGui = parent
        else
            parentGui = CoreGui:FindFirstChild("Rivals_Notifications")
            if not parentGui then
                local created = Instance.new("ScreenGui")
                created.Name = "Rivals_Notifications"
                created.ResetOnSpawn = false
                created.ZIndexBehavior = Enum.ZIndexBehavior.Sibling
                local ok = pcall(function() created.Parent = CoreGui end)
                if not ok then
                    if lp and lp:FindFirstChild("PlayerGui") then
                        created.Parent = lp:WaitForChild("PlayerGui")
                    else
                        pcall(function() created.Parent = CoreGui end)
                    end
                end
                pcall(function() created.DisplayOrder = 1000 end)
                parentGui = created
            end
        end
    end

    if NOTIFICATIONS_ENABLED == false then return nil end
    if type(NotificationAPI) == "table" and type(NotificationAPI.CanCreate) == "function" then
        if not NotificationAPI.CanCreate(invoker) then return nil end
    end

    local holder = parentGui:FindFirstChild("RivalsNotificationsHolder")
    if not holder then
        holder = Instance.new("Frame")
        holder.Name = "RivalsNotificationsHolder"
        holder.Size = UDim2.new(0, 420, 0, 200)
        holder.AnchorPoint = Vector2.new(1, 1)
        holder.Position = UDim2.new(1, -12, 1, -12)
        holder.BackgroundTransparency = 1
        holder.ZIndex = 10000
        holder.Parent = parentGui
        local layout = Instance.new("UIListLayout")
        layout.Name = "RivalsNotificationsLayout"
        layout.SortOrder = Enum.SortOrder.LayoutOrder
        layout.Padding = UDim.new(0, 8)
        layout.VerticalAlignment = Enum.VerticalAlignment.Bottom
        layout.Parent = holder
        local pad = Instance.new("UIPadding") pad.Parent = holder
        pad.PaddingRight = UDim.new(0, 0)
        pad.PaddingBottom = UDim.new(0, 0)
    end

    local TextService = game:GetService("TextService")
    local msgStr = tostring(text or "Notification")
    local fontSize = 16
    local font = Enum.Font.GothamBold
    local screenW = 760
    pcall(function() if gui and gui.Parent and gui.AbsoluteSize and gui.AbsoluteSize.X then screenW = gui.AbsoluteSize.X end end)
    local maxAllowed = math.max(240, screenW - 24)
    local maxContentW = math.min(600, maxAllowed - 80)
    local measured = TextService:GetTextSize(msgStr, fontSize, font, Vector2.new(maxContentW, 10000))
    local _tmpW = math.ceil(measured.X + 80)
    local targetW = math.min(math.max(_tmpW, 240), maxAllowed)
    local targetH = math.max(56, math.ceil(measured.Y + 24))

    local container = Instance.new("Frame")
    container.Name = "RivalsNotification"
    container.Size = UDim2.new(0, targetW, 0, targetH)
    container.BackgroundColor3 = COLORS.panelDark
    container.BorderSizePixel = 0
    container.ZIndex = holder.ZIndex
    container.LayoutOrder = math.floor(tick() * 1000)
    container.Parent = holder
    pcall(function()
        local hX = holder.Size.X.Offset or 420
        if targetW > hX then
            holder.Size = UDim2.new(0, targetW, holder.Size.Y.Scale, holder.Size.Y.Offset)
        end
    end)
    RegisterThemed(container)
    local cCorner = Instance.new("UICorner") cCorner.CornerRadius = UDim.new(0,10) cCorner.Parent = container
    local cStroke = Instance.new("UIStroke") cStroke.Color = COLORS.divider; cStroke.Thickness = 1; cStroke.Parent = container

    -- ** accent bar
    local accent = Instance.new("Frame")
    accent.Size = UDim2.new(0, 6, 1, 0)
    accent.Position = UDim2.new(0, 0, 0, 0)
    accent.BackgroundColor3 = COLORS.accent
    accent.BorderSizePixel = 0
    accent.ZIndex = container.ZIndex + 2
    accent.Parent = container
    local aCorner = Instance.new("UICorner") aCorner.CornerRadius = UDim.new(0,4) aCorner.Parent = accent
    RegisterThemed(accent)

    local inner = Instance.new("Frame")
    inner.Size = UDim2.new(1, -20, 1, -12)
    inner.Position = UDim2.new(0, 12, 0, 6)
    inner.BackgroundTransparency = 1
    inner.ZIndex = container.ZIndex + 1
    inner.Parent = container

    local icon = Instance.new("TextLabel")
    icon.Size = UDim2.new(0, 28, 0, 28)
    icon.Position = UDim2.new(0, 0, 0.5, -14)
    icon.BackgroundTransparency = 1
    icon.Font = Enum.Font.GothamBold
    icon.TextSize = 18
    icon.TextColor3 = COLORS.accent
    icon.Text = "🔔"
    icon.ZIndex = inner.ZIndex + 1
    icon.Parent = inner

    local label = Instance.new("TextLabel")
    label.Size = UDim2.new(1, -36, 1, 0)
    label.Position = UDim2.new(0, 36, 0, 0)
    label.BackgroundTransparency = 1
    label.Font = font
    label.TextSize = fontSize
    label.Text = msgStr
    label.TextColor3 = COLORS.text
    label.TextStrokeTransparency = 0.7
    label.TextXAlignment = Enum.TextXAlignment.Left
    label.TextYAlignment = Enum.TextYAlignment.Center
    label.TextWrapped = true
    label.ZIndex = inner.ZIndex + 1
    label.Parent = inner

    -- ** progress bar
    local barHolder = Instance.new("Frame")
    barHolder.Size = UDim2.new(1, -20, 0, 6)
    barHolder.Position = UDim2.new(0, 10, 1, -10)
    barHolder.BackgroundTransparency = 1
    barHolder.ZIndex = container.ZIndex + 1
    barHolder.Parent = container

    local prog = Instance.new("Frame")
    prog.AnchorPoint = Vector2.new(1, 0)
    prog.Position = UDim2.new(1, 0, 0, 0)
    prog.Size = UDim2.new(1, 0, 1, 0)
    prog.BackgroundColor3 = COLORS.accent
    prog.BorderSizePixel = 0
    prog.ZIndex = container.ZIndex + 2
    prog.Parent = barHolder
    local progCorner = Instance.new("UICorner") progCorner.CornerRadius = UDim.new(0,3) progCorner.Parent = prog

    -- ** entrance animation (pop)
    pcall(function()
        container.Size = UDim2.new(0, targetW, 0, 0)
        container.Position = container.Position
        TweenService:Create(container, TweenInfo.new(0.34, Enum.EasingStyle.Elastic, Enum.EasingDirection.Out), {Size = UDim2.new(0, targetW, 0, targetH)}):Play()
        -- ** fade/slide inner
        label.TextTransparency = 1
        icon.TextTransparency = 1
        TweenService:Create(label, TweenInfo.new(0.28, Enum.EasingStyle.Quad), {TextTransparency = 0}):Play()
        TweenService:Create(icon, TweenInfo.new(0.28, Enum.EasingStyle.Quad), {TextTransparency = 0}):Play()
    end)

    -- ** prog bar tween
    local progTween = TweenService:Create(prog, TweenInfo.new(dur, Enum.EasingStyle.Linear), {Size = UDim2.new(0, 0, 1, 0)})
    progTween:Play()

    -- ** auto destroy after duration with exit animation
    task.delay(dur, function()
        pcall(function()
            TweenService:Create(label, TweenInfo.new(0.22, Enum.EasingStyle.Quad), {TextTransparency = 1}):Play()
            TweenService:Create(icon, TweenInfo.new(0.22, Enum.EasingStyle.Quad), {TextTransparency = 1}):Play()
            TweenService:Create(container, TweenInfo.new(0.28, Enum.EasingStyle.Quad), {Size = UDim2.new(0, 420, 0, 0)}):Play()
        end)
        task.delay(0.32, function()
            pcall(function() container:Destroy() end)
        end)
    end)

    return container
end


spawn(function()
    for i=1,60 do
        local api = ToggleAPI[enableNotificationsToggle]
        if api then
            local prev = api.OnToggle
            api.OnToggle = function(state)
                if prev then pcall(prev, state) end
                if not state then
                    pcall(function()
                        local CoreGui = game:GetService("CoreGui")
                        local Players = game:GetService("Players")
                        local root = CoreGui:FindFirstChild("Rivals_Notifications")
                        if not root and Players and Players.LocalPlayer then
                            local pg = Players.LocalPlayer:FindFirstChild("PlayerGui")
                            if pg then root = pg:FindFirstChild("Rivals_Notifications") end
                        end
                        if root then
                            local holder = root:FindFirstChild("RivalsNotificationsHolder")
                            if holder then holder:Destroy() end
                        end
                    end)
                end
            end
            break
        end
        task.wait(0.1)
    end
end)

--------------------------------------------------------------------------

-- ** makeButton
local function makeButton(parent, labelText)
    local frame = Instance.new("Frame")
    frame.Name = tostring(labelText or "Button")
    frame.Size = UDim2.new(1, 0, 0, 34)
    frame.BackgroundTransparency = 1
    frame.Parent = parent

    local label = Instance.new("TextLabel")
    label.Size = UDim2.new(0.72, -6, 1, 0)
    label.BackgroundTransparency = 1
    label.Text = labelText or "Button"
    label.Font = Enum.Font.GothamBold
    label.TextSize = 18
    label.TextColor3 = COLORS.text
    label.TextXAlignment = Enum.TextXAlignment.Left
    label.Parent = frame

    local btn = Instance.new("TextButton")
    btn.Size = UDim2.new(0, 84, 0, 26)
    btn.AnchorPoint = Vector2.new(1,0.5)
    btn.Position = UDim2.new(1, -8, 0.5, 0)
    btn.BackgroundColor3 = COLORS.panelDark
    btn.AutoButtonColor = true
    btn.Font = Enum.Font.Gotham
    btn.TextSize = 16
    btn.TextColor3 = COLORS.text
    btn.Text = "Click"
    btn.Parent = frame
    local btnCorner = Instance.new("UICorner")
    btnCorner.CornerRadius = UDim.new(0,6)
    btnCorner.Parent = btn

    -- ** Register API entry
    if type(ButtonAPI) ~= "table" then
        ButtonAPI = setmetatable({}, { __mode = "k" })
    end
    ButtonAPI[frame] = {
        OnClick = nil,
        Click = function()
            local api = ButtonAPI[frame]
            if api and type(api.OnClick) == "function" then pcall(api.OnClick) end
        end,
    }

    btn.MouseButton1Click:Connect(function()
        local api = ButtonAPI[frame]
        if api and type(api.OnClick) == "function" then pcall(api.OnClick) end
    end)

    local maxOrder = 0
    for _,c in ipairs(parent:GetChildren()) do
        if c ~= frame and (c:IsA("Frame") or c:IsA("TextLabel") or c:IsA("TextButton")) then
            maxOrder = math.max(maxOrder, c.LayoutOrder or 0)
        end
    end
    frame.LayoutOrder = maxOrder + 1

    return frame
end

--------------------------------------------------------------------------

-- ** Collapsible group helper
local TweenService = game:GetService("TweenService")
local function makeCollapsibleGroup(parent, title, defaultOpen, builderFn)
    local headerHeight = 36
    local extraWidth = 8 
    local extraX = -10 
    local grp = Instance.new("Frame")
    grp.Name = tostring(title or "Group")
    grp.BackgroundTransparency = 1
    grp.Size = UDim2.new(1, extraWidth, 0, headerHeight)
    grp.Position = UDim2.new(0, extraX, 0, 0)
    grp.Parent = parent

    local header = Instance.new("TextButton")
    header.Size = UDim2.new(1, 0, 0, headerHeight)
    header.Position = UDim2.new(0,0,0,0)
    header.BackgroundColor3 = COLORS.panelAlt or COLORS.panel
    header.AutoButtonColor = false
    header.Font = Enum.Font.GothamBold
    header.TextSize = 18
    header.Text = tostring(title or "Group")
    header.TextXAlignment = Enum.TextXAlignment.Left
    header.TextColor3 = COLORS.text
    header.Parent = grp
    header.ZIndex = 50
    local hp = Instance.new("UIPadding") hp.Parent = header hp.PaddingLeft = UDim.new(0,12); hp.PaddingRight = UDim.new(0,28)
    local hcorner = Instance.new("UICorner") hcorner.CornerRadius = UDim.new(0,6) hcorner.Parent = header
    local hstroke = Instance.new("UIStroke") hstroke.Parent = header hstroke.Color = COLORS.panelStroke or COLORS.panel
    RegisterThemed(header)

    local caret = Instance.new("TextLabel")
    caret.Size = UDim2.new(0, 18, 0, 18)
    caret.AnchorPoint = Vector2.new(1, 0.5)
    caret.Position = UDim2.new(1, -12, 0.5, 0)
    caret.BackgroundTransparency = 1
    caret.Font = Enum.Font.Gotham
    caret.TextSize = 16
    caret.Text = "▾"
    caret.TextColor3 = COLORS.textDim
    caret.ZIndex = header.ZIndex + 1
    caret.Parent = header
    RegisterThemed(caret)

    local bodyClip = Instance.new("Frame")
    bodyClip.Name = "BodyClip"
    bodyClip.BackgroundTransparency = 1
    bodyClip.Position = UDim2.new(0,0,0,headerHeight)
    bodyClip.Size = UDim2.new(1,0,0,0)
    bodyClip.ClipsDescendants = true
    bodyClip.Parent = grp

    local inner = Instance.new("Frame")
    inner.Name = "Inner"
    inner.BackgroundTransparency = 1
    inner.Size = UDim2.new(1,0,0,0)
    inner.AutomaticSize = Enum.AutomaticSize.Y
    inner.Parent = bodyClip

    local innerLayout = Instance.new("UIListLayout") innerLayout.Parent = inner
    innerLayout.SortOrder = Enum.SortOrder.LayoutOrder
    innerLayout.Padding = UDim.new(0,6)
    local innerPad = Instance.new("UIPadding") innerPad.Parent = inner
    innerPad.PaddingLeft = UDim.new(0,4); innerPad.PaddingRight = UDim.new(0,4); innerPad.PaddingTop = UDim.new(0,8); innerPad.PaddingBottom = UDim.new(0,8)
    RegisterThemed(inner)

    if type(builderFn) == "function" then
        pcall(builderFn, inner)
    end

    local opened = not not defaultOpen
    local tweenInfo = TweenInfo.new(0.18, Enum.EasingStyle.Quad, Enum.EasingDirection.Out)

    local function refreshSizes()
        local contentH = innerLayout.AbsoluteContentSize.Y
        if opened then
            grp.Size = UDim2.new(1, extraWidth, 0, headerHeight + contentH)
            bodyClip.Size = UDim2.new(1,0,0, contentH)
            caret.Text = "▾"
            caret.Rotation = 0
        else
            grp.Size = UDim2.new(1, extraWidth, 0, headerHeight)
            bodyClip.Size = UDim2.new(1,0,0,0)
            caret.Text = "▸"
            caret.Rotation = -90
        end
    end

    task.defer(refreshSizes)

    local function setOpen(open)
        opened = not not open
        local contentH = innerLayout.AbsoluteContentSize.Y
        if opened then
            TweenService:Create(grp, tweenInfo, {Size = UDim2.new(1, extraWidth, 0, headerHeight + contentH)}):Play()
            TweenService:Create(bodyClip, tweenInfo, {Size = UDim2.new(1,0,0, contentH)}):Play()
            TweenService:Create(caret, tweenInfo, {Rotation = 0}):Play()
            caret.Text = "▾"
        else
            TweenService:Create(grp, tweenInfo, {Size = UDim2.new(1, extraWidth, 0, headerHeight)}):Play()
            TweenService:Create(bodyClip, tweenInfo, {Size = UDim2.new(1,0,0,0)}):Play()
            TweenService:Create(caret, tweenInfo, {Rotation = -90}):Play()
            caret.Text = "▸"
        end
        if bottomDivider then
            if opened then
                pcall(function() TweenService:Create(bottomDivider, tweenInfo, {BackgroundTransparency = 0}):Play() end)
            else
                pcall(function() TweenService:Create(bottomDivider, tweenInfo, {BackgroundTransparency = 1}):Play() end)
            end
        end
    end

    header.MouseButton1Click:Connect(function()
        setOpen(not opened)
    end)

    local bottomDivider = Instance.new("Frame")
    bottomDivider.Name = "BottomDivider"
    bottomDivider.Size = UDim2.new(1, 0, 0, 1)
    bottomDivider.Position = UDim2.new(0, 0, 1, 6)
    bottomDivider.AnchorPoint = Vector2.new(0, 1)
    bottomDivider.BackgroundColor3 = COLORS.divider or (COLORS.panel or COLORS.bg)
    bottomDivider.BorderSizePixel = 0
    bottomDivider.ZIndex = 1
    bottomDivider.Parent = grp
    bottomDivider.BackgroundTransparency = (opened and 0 or 1)
    RegisterThemed(bottomDivider, function()
        pcall(function() bottomDivider.BackgroundColor3 = COLORS.divider or (COLORS.panel or COLORS.bg) end)
    end)

    return {
        SetOpen = setOpen,
        Toggle = function() setOpen(not opened) end,
        Add = function(fn) if type(fn) == "function" then pcall(fn, inner) end end,
        Header = header,
        Body = inner,
        Frame = grp,
    }
end

-- ** makeSlider

local function makeSlider(parent, labelText, minVal, maxVal, defaultVal)
    local MIN = (type(minVal) == "number") and minVal or 1
    local MAX = (type(maxVal) == "number") and maxVal or 100
    local initial = (type(defaultVal) == "number") and defaultVal or math.floor((MIN + MAX) / 2)

    local frame = Instance.new("Frame")
    frame.Name = tostring(labelText or "Slider")
    frame.Size = UDim2.new(1, 0, 0, 34)
    frame.BackgroundTransparency = 1
    frame.Parent = parent
    RegisterThemed(frame)

    local label = Instance.new("TextLabel")
    label.Size = UDim2.new(0.4, -6, 1, 0)
    label.BackgroundTransparency = 1
    label.Text = labelText or "Slider"
    label.Font = Enum.Font.GothamBold
    label.TextSize = 18
    label.TextColor3 = COLORS.text
    label.TextXAlignment = Enum.TextXAlignment.Left
    label.Parent = frame

    local holder = Instance.new("Frame")
    holder.AnchorPoint = Vector2.new(1, 0)
    holder.Position = UDim2.new(1, -8, 0, 2)
    holder.Size = UDim2.new(0.6, -8, 1, -4)
    holder.BackgroundTransparency = 1
    holder.Parent = frame

    local bar = Instance.new("Frame")
    bar.Name = "Bar"
    bar.Size = UDim2.new(1, 0, 0, 12)
    bar.Position = UDim2.new(0, 0, 0.5, -6)
    bar.BackgroundColor3 = COLORS.panelDark
    bar.BorderSizePixel = 0
    bar.Parent = holder
    RegisterThemed(bar)
    local barCorner = Instance.new("UICorner") barCorner.CornerRadius = UDim.new(0,6) barCorner.Parent = bar

    local fill = Instance.new("Frame")
    fill.Name = "Fill"
    fill.Size = UDim2.new(0, 0, 1, 0)
    fill.BackgroundColor3 = COLORS.accent
    fill.BorderSizePixel = 0
    fill.Parent = bar
    RegisterThemed(fill)
    local fillCorner = Instance.new("UICorner") fillCorner.CornerRadius = UDim.new(0,6) fillCorner.Parent = fill

    local handle = Instance.new("TextButton")
    handle.Name = "Handle"
    handle.Size = UDim2.new(0, 16, 0, 16)
    handle.Position = UDim2.new(0, -8, 0.5, -8)
    handle.AnchorPoint = Vector2.new(0.5, 0.5)
    handle.AutoButtonColor = false
    handle.BackgroundColor3 = COLORS.panel
    handle.Text = ""
    handle.Parent = bar
    RegisterThemed(handle)
    local handleCorner = Instance.new("UICorner") handleCorner.CornerRadius = UDim.new(0,8) handleCorner.Parent = handle

    local valueLabel = Instance.new("TextLabel")
    valueLabel.Name = "Value"
    valueLabel.Size = UDim2.new(0.5, 0, 1, 0)
    valueLabel.Position = UDim2.new(0.25, 0, 0, 0)
    valueLabel.BackgroundTransparency = 1
    valueLabel.Font = Enum.Font.GothamBold
    valueLabel.TextSize = 14
    valueLabel.TextColor3 = COLORS.text
    valueLabel.Text = tostring(initial)
    valueLabel.Parent = holder
    valueLabel.TextXAlignment = Enum.TextXAlignment.Center
    valueLabel.TextYAlignment = Enum.TextYAlignment.Center

    -------------------- Continue from UI --------------------

    -- ** internal state
    local dragging = false
    local current = math.clamp(initial, MIN, MAX)

    local function setValue(v)
        v = math.floor(math.clamp(v or MIN, MIN, MAX))
        local prev = current
        current = v
        local pct = 0
        if MAX > MIN then pct = (current - MIN) / (MAX - MIN) end
        fill.Size = UDim2.new(pct, 0, 1, 0)
        handle.Position = UDim2.new(pct, 0, 0.5, 0)
        valueLabel.Text = tostring(current)
        if current ~= prev then
            local api = SliderAPI[frame]
            if api and type(api.OnChange) == "function" then pcall(api.OnChange, current) end
        end
    end

    local function inputToValue(inputX)
        local absPos = inputX - bar.AbsolutePosition.X
        local w = bar.AbsoluteSize.X
        local pct = 0
        if w > 0 then pct = math.clamp(absPos / w, 0, 1) end
        local v = math.floor(MIN + pct * (MAX - MIN) + 0.5)
        return v
    end

    handle.InputBegan:Connect(function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            dragging = true
            pcall(function() handle:CaptureFocus() end)
        end
    end)

    handle.InputEnded:Connect(function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            dragging = false
            pcall(function() handle:ReleaseFocus() end)
        end
    end)

    UserInputService.InputChanged:Connect(function(input)
        if not dragging then return end
        if input.UserInputType == Enum.UserInputType.MouseMovement then
            local v = inputToValue(input.Position.X)
            setValue(v)
        end
    end)

    bar.InputBegan:Connect(function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            local v = inputToValue(input.Position.X)
            setValue(v)
        end
    end)

    -- ** API
    SliderAPI[frame] = {
        Get = function() return current end,
        Set = function(v) setValue(v) end,
        OnChange = nil,
        Min = MIN,
        Max = MAX,
    }

    local maxOrder = 0
    for _,c in ipairs(parent:GetChildren()) do
        if c ~= frame and (c:IsA("Frame") or c:IsA("TextLabel") or c:IsA("TextButton")) then
            maxOrder = math.max(maxOrder, c.LayoutOrder or 0)
        end
    end
    frame.LayoutOrder = maxOrder + 1

    if bar.AbsoluteSize and bar.AbsoluteSize.X > 0 then
        pcall(setValue, current)
    else
        local conn
        conn = bar:GetPropertyChangedSignal("AbsoluteSize"):Connect(function()
            if bar.AbsoluteSize and bar.AbsoluteSize.X > 0 then
                pcall(setValue, current)
                pcall(function() if conn and conn.Disconnect then conn:Disconnect() end end)
            end
        end)
        task.delay(0.1, function()
            pcall(setValue, current)
            pcall(function() if conn and conn.Disconnect then conn:Disconnect() end end)
        end)
    end
    return frame
end

--------------------------------------------------------------------------


-- ** makeKeyBindButton
local function makeKeyBindButton(parent, title, defaultKey)
    local frame = Instance.new("Frame")
    frame.Size = UDim2.new(1,0,0,34)
    frame.BackgroundTransparency = 1
    frame.Parent = parent

    local label = Instance.new("TextLabel")
    label.Size = UDim2.new(0.5, -6, 1, 0)
    label.BackgroundTransparency = 1
    label.Text = title or "Keybind"
    label.Font = Enum.Font.GothamBold
    label.TextSize = 18
    label.TextColor3 = COLORS.text
    label.TextXAlignment = Enum.TextXAlignment.Left
    label.Parent = frame

    local btn = Instance.new("TextButton")
    btn.Name = frame.Name .. "_Bind"
    btn.Size = UDim2.new(0.5, -8, 1, 0)
    btn.AnchorPoint = Vector2.new(1,0)
    btn.Position = UDim2.new(1, -8, 0, 0)
    btn.BackgroundColor3 = COLORS.panelDark
    btn.AutoButtonColor = true
    btn.Font = Enum.Font.Gotham
    btn.TextSize = 16
    btn.TextColor3 = COLORS.text
    btn.Text = "None"
    btn.Parent = frame
    local btnCorner = Instance.new("UICorner") btnCorner.CornerRadius = UDim.new(0,6) btnCorner.Parent = btn

    local function keyName(k)
        if not k then return "None" end
        if typeof(k) == "EnumItem" then return k.Name end
        return tostring(k)
    end

    local current = nil
    if defaultKey then
        if typeof(defaultKey) == "EnumItem" then current = defaultKey end
    end

    local listening = false
    local pending = nil
    local inputConn = nil

    local function updateText()
        if listening then
            btn.Text = 'Press enter to save keybind to "' .. (title or "keybind") .. '"!'
        else
            btn.Text = keyName(current)
        end
    end

    btn.MouseButton1Click:Connect(function()
        if listening then return end
        listening = true
        pending = nil
        updateText()
        task.wait(0.05)
        inputConn = UserInputService.InputBegan:Connect(function(input, gameProcessed)
            if gameProcessed then return end
            if input.UserInputType ~= Enum.UserInputType.Keyboard then return end
            local kc = input.KeyCode
            if kc == Enum.KeyCode.Unknown then return end
            if kc == Enum.KeyCode.Return or kc == Enum.KeyCode.KeypadEnter then
                if pending then
                    current = pending
                    local api = KeybindAPI[frame]
                    if api and type(api.OnBind) == "function" then
                        pcall(api.OnBind, current)
                    end
                end
                listening = false
                if inputConn then inputConn:Disconnect() inputConn = nil end
                updateText()
            elseif kc == Enum.KeyCode.Escape then
                listening = false
                pending = nil
                if inputConn then inputConn:Disconnect() inputConn = nil end
                updateText()
            else
                pending = kc
                btn.Text = kc.Name .. " (Press Enter to save)"
            end
        end)
    end)

    KeybindAPI[frame] = {
        Get = function() return current end,
        Set = function(k)
            if typeof(k) == "EnumItem" then current = k else current = nil end
            updateText()
        end,
        OnBind = nil,
    }

    -- ** layout order
    local maxOrder = 0
    for _,c in ipairs(parent:GetChildren()) do
        if c:IsA("Frame") or c:IsA("TextLabel") or c:IsA("TextButton") then
            if c.LayoutOrder and c.LayoutOrder > maxOrder then maxOrder = c.LayoutOrder end
        end
    end
    frame.LayoutOrder = maxOrder + 1

    updateText()
    return frame
end

--------------------------------------------------------------------------

-- ** makeDropDownList
local function makeDropDownList(parent, labelText, items, defaultIndex)
    local frame = Instance.new("Frame")
    frame.Name = tostring(labelText or "DropDown")
    frame.Size = UDim2.new(1, 0, 0, 34)
    frame.BackgroundTransparency = 1
    frame.Parent = parent
    RegisterThemed(frame)

    local label = Instance.new("TextLabel")
    label.Size = UDim2.new(0.4, -6, 1, 0)
    label.BackgroundTransparency = 1
    label.Text = labelText or "Select"
    label.Font = Enum.Font.GothamBold
    label.TextSize = 18
    label.TextColor3 = COLORS.text
    label.TextXAlignment = Enum.TextXAlignment.Left
    label.Parent = frame

    local display = Instance.new("TextButton")
    display.Name = frame.Name .. "_Display"
    display.Size = UDim2.new(0.4, -8, 1, 0)
    display.AnchorPoint = Vector2.new(1, 0)
    display.Position = UDim2.new(1, -8, 0, 0)
    display.BackgroundColor3 = COLORS.panelDark
    display.AutoButtonColor = false
    display.Font = Enum.Font.Gotham
    display.TextSize = 16
    display.TextColor3 = COLORS.text
    display.Text = ""
    display.TextXAlignment = Enum.TextXAlignment.Left
    display.Parent = frame
    RegisterThemed(display)
    local displayCorner = Instance.new("UICorner") displayCorner.CornerRadius = UDim.new(0,6) displayCorner.Parent = display
    local displayPad = Instance.new("UIPadding") displayPad.Parent = display
    displayPad.PaddingLeft = UDim.new(0,8)
    displayPad.PaddingRight = UDim.new(0,28)
    display.Active = true

    local arrow = Instance.new("TextLabel")
    arrow.Size = UDim2.new(0, 24, 1, 0)
    arrow.AnchorPoint = Vector2.new(1,0.5)
    arrow.Position = UDim2.new(1, -4, 0.5, 0)
    arrow.BackgroundTransparency = 1
    arrow.Font = Enum.Font.Gotham
    arrow.TextSize = 18
    arrow.TextColor3 = COLORS.textDim
    arrow.Text = "▾"
    arrow.Parent = display

    local drop = Instance.new("Frame")
    drop.Size = UDim2.new(1, 0, 0, 0)
    drop.Position = UDim2.new(0, 0, 1, 6)
    drop.BackgroundColor3 = COLORS.panelAlt
    drop.ClipsDescendants = true
    drop.Visible = false
    local DROP_ZINDEX = 50
    drop.ZIndex = DROP_ZINDEX
    drop.Parent = frame
    RegisterThemed(drop)
    local dropCorner = Instance.new("UICorner") dropCorner.CornerRadius = UDim.new(0,8) dropCorner.Parent = drop
    local dropStroke = Instance.new("UIStroke") dropStroke.Thickness = 1; dropStroke.Color = COLORS.divider; dropStroke.Parent = drop

    local scroll = Instance.new("ScrollingFrame")
    scroll.Size = UDim2.new(1, -12, 1, -12)
    scroll.Position = UDim2.new(0, 6, 0, 6)
    scroll.BackgroundTransparency = 1
    scroll.ScrollBarThickness = 8
    pcall(function() scroll.ScrollBarImageColor3 = COLORS.accent end)
    scroll.Parent = drop
    scroll.ZIndex = DROP_ZINDEX
    local layout = Instance.new("UIListLayout") layout.Parent = scroll
    layout.SortOrder = Enum.SortOrder.LayoutOrder
    layout.Padding = UDim.new(0, 4)
    local scrollPad = Instance.new("UIPadding") scrollPad.Parent = scroll
    scrollPad.PaddingTop = UDim.new(0,4); scrollPad.PaddingBottom = UDim.new(0,4)

    items = items or {}
    local selected = nil
    local btnRefs = {}
    local selectedIndices = {}

    local function populate()
        for _,c in ipairs(scroll:GetChildren()) do
            if c:IsA("TextButton") then c:Destroy() end
        end
        for i, v in ipairs(items) do
            local btn = Instance.new("TextButton")
            btn.Size = UDim2.new(1, 0, 0, 28)
            btn.BackgroundTransparency = 1
            btn.AutoButtonColor = false
            btn.Font = Enum.Font.Gotham
            btn.TextSize = 16
            btn.TextColor3 = COLORS.text
            btn.Text = tostring(v)
            btn.LayoutOrder = i
            btn.Parent = scroll
            btn.ZIndex = DROP_ZINDEX + 1
            local btnCorner = Instance.new("UICorner") btnCorner.CornerRadius = UDim.new(0,6) btnCorner.Parent = btn
            local btnPad = Instance.new("UIPadding") btnPad.Parent = btn; btnPad.PaddingLeft = UDim.new(0,8)

            btnRefs[i] = btn
            selectedIndices[i] = false

            local function updateBtnVisual(idx)
                local b = btnRefs[idx]
                if not b then return end
                if selectedIndices[idx] then
                    b.BackgroundTransparency = 0
                    b.BackgroundColor3 = COLORS.highlight
                    b.TextColor3 = COLORS.white
                else
                    b.BackgroundTransparency = 1
                    b.BackgroundColor3 = COLORS.panel
                    b.TextColor3 = COLORS.text
                end
            end

            btn.MouseEnter:Connect(function()
                if selectedIndices[i] then return end
                pcall(function() TweenService:Create(btn, TweenInfo.new(0.12), {BackgroundTransparency = 0, BackgroundColor3 = COLORS.panelAlt}):Play() end)
            end)
            btn.MouseLeave:Connect(function()
                if selectedIndices[i] then return end
                pcall(function() TweenService:Create(btn, TweenInfo.new(0.12), {BackgroundTransparency = 1}):Play() end)
            end)

            btn.MouseButton1Click:Connect(function()
                selectedIndices[i] = not selectedIndices[i]
                updateBtnVisual(i)
                selected = { index = i, value = v }
                display.Text = tostring(v)
                pcall(function() drop.Visible = false; TweenService:Create(drop, TweenInfo.new(0.12), {Size = UDim2.new(1,0,0,0)}):Play() end)
                arrow.Text = "▾"
                pcall(function() TweenService:Create(arrow, TweenInfo.new(0.12), {TextColor3 = COLORS.textDim}):Play() end)
                local api = DropdownAPI[frame]
                if api and type(api.OnSelect) == "function" then pcall(api.OnSelect, i, v, selectedIndices[i]) end
            end)
        end
        local total = #items * 28
        drop.Size = UDim2.new(1, 0, 0, math.min(total, 200))
    end

    display.MouseButton1Click:Connect(function()
        local open = not drop.Visible
        local total = #items * 28
        local target = math.min(total, 200)
        if open then
            drop.Visible = true
            TweenService:Create(drop, TweenInfo.new(0.18, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Size = UDim2.new(1,0,0,target)}):Play()
            arrow.Text = "▴"
            pcall(function() TweenService:Create(arrow, TweenInfo.new(0.18), {TextColor3 = COLORS.accent}):Play() end)
        else
            local tween = TweenService:Create(drop, TweenInfo.new(0.12, Enum.EasingStyle.Quad, Enum.EasingDirection.In), {Size = UDim2.new(1,0,0,0)})
            tween:Play()
            tween.Completed:Connect(function()
                pcall(function() drop.Visible = false end)
            end)
            arrow.Text = "▾"
            pcall(function() TweenService:Create(arrow, TweenInfo.new(0.12), {TextColor3 = COLORS.textDim}):Play() end)
        end
    end)

    -- ** API
    DropdownAPI[frame] = {
        SetItems = function(tbl) items = tbl or {} populate() end,
        Set = function(idx)
            local v = items[idx]
            if v ~= nil then
                selected = { index = idx, value = v }
                display.Text = tostring(v)
                for k,_ in pairs(selectedIndices) do
                    selectedIndices[k] = false
                    if btnRefs[k] then
                        pcall(function()
                            btnRefs[k].BackgroundTransparency = 1
                            btnRefs[k].BackgroundColor3 = COLORS.panel
                            btnRefs[k].TextColor3 = COLORS.text
                        end)
                    end
                end
                selectedIndices[idx] = true
                if btnRefs[idx] then
                    pcall(function()
                        btnRefs[idx].BackgroundTransparency = 0
                        btnRefs[idx].BackgroundColor3 = COLORS.highlight
                        btnRefs[idx].TextColor3 = COLORS.white
                    end)
                end
            end
        end,
        Get = function() return selected end,
        SetSelected = function(idx, on)
            selectedIndices[idx] = (on == true)
            if btnRefs[idx] then
                if selectedIndices[idx] then
                    pcall(function()
                        btnRefs[idx].BackgroundTransparency = 0
                        btnRefs[idx].BackgroundColor3 = COLORS.highlight
                        btnRefs[idx].TextColor3 = COLORS.white
                    end)
                else
                    pcall(function()
                        btnRefs[idx].BackgroundTransparency = 1
                        btnRefs[idx].BackgroundColor3 = COLORS.panel
                        btnRefs[idx].TextColor3 = COLORS.text
                    end)
                end
            end
        end,
        IsSelected = function(idx) return selectedIndices[idx] == true end,
        OnSelect = nil,
    }

    populate()
    if defaultIndex then DropdownAPI[frame].Set(defaultIndex) end

    -- ** layout order
    local maxOrder = 0
    for _,c in ipairs(parent:GetChildren()) do
        if c ~= frame and (c:IsA("Frame") or c:IsA("TextLabel") or c:IsA("TextButton")) then
            maxOrder = math.max(maxOrder, c.LayoutOrder or 0)
        end
    end
    frame.LayoutOrder = maxOrder + 1

    return frame
end


--------------------------------------------------------------------------

-- ** makeColorPicker

local function makeColorPicker(parent, labelText, defaultColor)
    local frame = Instance.new("Frame")
    frame.Name = tostring(labelText or "ColorPicker")
    frame.Size = UDim2.new(1, 0, 0, 34)
    frame.BackgroundTransparency = 1
    frame.Parent = parent
    RegisterThemed(frame)

    local label = Instance.new("TextLabel")
    label.Size = UDim2.new(0.5, -6, 1, 0)
    label.BackgroundTransparency = 1
    label.Text = labelText or "Color"
    label.Font = Enum.Font.GothamBold
    label.TextSize = 18
    label.TextColor3 = COLORS.text
    label.TextXAlignment = Enum.TextXAlignment.Left
    label.Parent = frame

    local display = Instance.new("TextButton")
    display.Size = UDim2.new(0.48, 0, 1, 0)
    display.AnchorPoint = Vector2.new(1, 0)
    display.Position = UDim2.new(1, 0, 0, 0)
    display.BackgroundColor3 = COLORS.panelDark
    display.BorderSizePixel = 0
    display.AutoButtonColor = false
    display.Parent = frame
    RegisterThemed(display)
    local dispCorner = Instance.new("UICorner") dispCorner.CornerRadius = UDim.new(0,6) dispCorner.Parent = display
    local dispPad = Instance.new("UIPadding") dispPad.Parent = display; dispPad.PaddingLeft = UDim.new(0,8)

    local swatch = Instance.new("Frame")
    swatch.Size = UDim2.new(0, 20, 0, 20)
    swatch.Position = UDim2.new(0, 0, 0.5, -10)
    swatch.BackgroundColor3 = (type(defaultColor) == "Color3") and defaultColor or COLORS.accent
    swatch.BorderSizePixel = 0
    swatch.Parent = display
    RegisterThemed(swatch)
    local swCorner = Instance.new("UICorner") swCorner.CornerRadius = UDim.new(0,4) swCorner.Parent = swatch

    local arrow = Instance.new("TextLabel")
    arrow.Size = UDim2.new(0,24,1,0)
    arrow.AnchorPoint = Vector2.new(1,0.5)
    arrow.Position = UDim2.new(1, -8, 0.5, 0)
    arrow.BackgroundTransparency = 1
    arrow.Font = Enum.Font.Gotham
    arrow.TextSize = 18
    arrow.TextColor3 = COLORS.textDim
    arrow.Text = "▾"
    arrow.Parent = display

    local palette = Instance.new("Frame")
    palette.Size = UDim2.new(1, 0, 0, 0)
    palette.Position = UDim2.new(0, 0, 1, 6)
    palette.BackgroundColor3 = COLORS.panelAlt
    palette.ClipsDescendants = true
    palette.Visible = false
    palette.Parent = frame
    local TOP_Z = 600
    palette.ZIndex = TOP_Z
    RegisterThemed(palette)
    local palCorner = Instance.new("UICorner") palCorner.CornerRadius = UDim.new(0,8) palCorner.Parent = palette
    local palStroke = Instance.new("UIStroke") palStroke.Thickness = 1; palStroke.Color = COLORS.divider; palStroke.Parent = palette

    local scroll = Instance.new("Frame")
    scroll.Size = UDim2.new(1, -12, 0, 56)
    scroll.Position = UDim2.new(0, 6, 0, 6)
    scroll.BackgroundTransparency = 1
    scroll.Parent = palette
    scroll.ZIndex = TOP_Z

    local slidersHolder = Instance.new("Frame")
    slidersHolder.Size = UDim2.new(1, -12, 0, 36 * 3)
    slidersHolder.Position = UDim2.new(0, 6, 0, 68)
    slidersHolder.BackgroundTransparency = 1
    slidersHolder.Parent = palette
    slidersHolder.ZIndex = TOP_Z

    local function colorToHSVtbl(c)
        local ok, h, s, v = pcall(function() return Color3.toHSV(c) end)
        if ok and h then return {h = h * 360, s = s * 100, v = v * 100} end
        return {h = 200, s = 100, v = 100}
    end

    local initialHSV = colorToHSVtbl((type(defaultColor) == "Color3") and defaultColor or COLORS.accent)

    local current = swatch.BackgroundColor3

    local wheelSize = 180
    local wheelFrame = Instance.new("Frame")
    wheelFrame.Size = UDim2.new(0, wheelSize, 0, wheelSize)
    wheelFrame.Position = UDim2.new(0, 40, 0, 6)
    wheelFrame.BackgroundTransparency = 1
    wheelFrame.Parent = scroll
    wheelFrame.ZIndex = TOP_Z

    local RES = 64
    local cellSize = wheelSize / RES
    local half = wheelSize / 2
    local radius = half

    local intSize = math.ceil(cellSize) + 1
    for y = 0, RES - 1 do
        for x = 0, RES - 1 do
            local px = math.floor(x * cellSize)
            local py = math.floor(y * cellSize)
            local cx = (px + intSize * 0.5) - half
            local cy = (py + intSize * 0.5) - half
            local dist = math.sqrt(cx * cx + cy * cy)
            if dist <= radius + 1 then
                local ang = math.atan2(cy, cx)
                local hue = ((ang / (2 * math.pi)) + 0.5) % 1
                local sat = math.clamp(dist / radius, 0, 1)
                local val = 1
                local col = Color3.fromHSV(hue, sat, val)
                local cell = Instance.new("Frame")
                cell.Size = UDim2.new(0, intSize, 0, intSize)
                cell.Position = UDim2.new(0, px, 0, py)
                cell.BackgroundColor3 = col
                cell.BorderSizePixel = 0
                cell.Parent = wheelFrame
                cell.ZIndex = TOP_Z
            end
        end
    end

    local pointer = Instance.new("Frame")
    pointer.Size = UDim2.new(0, 12, 0, 12)
    pointer.AnchorPoint = Vector2.new(0.5, 0.5)
    pointer.BackgroundTransparency = 1
    pointer.Parent = wheelFrame
    local pCorner = Instance.new("UICorner") pCorner.CornerRadius = UDim.new(1,0) pCorner.Parent = pointer
    local pStroke = Instance.new("UIStroke") pStroke.Thickness = 2 pStroke.Color = Color3.new(0,0,0) pStroke.Parent = pointer
    pointer.ZIndex = TOP_Z + 1

    local function setColor(c)
        if not c then return end
        current = c
        swatch.BackgroundColor3 = c
        if ColorPickerAPI[frame] and type(ColorPickerAPI[frame].OnChange) == "function" then
            pcall(ColorPickerAPI[frame].OnChange, c)
        end
    end

    local function posToColor(px, py)
        local cx = px - half
        local cy = py - half
        local dist = math.sqrt(cx * cx + cy * cy)
        local sat = math.clamp(dist / radius, 0, 1)
        local ang = math.atan2(cy, cx)
        local hue = ((ang / (2 * math.pi)) + 0.5) % 1
        local color = Color3.fromHSV(hue, sat, 1)
        return color, hue, sat
    end

    local dragging = false
    local function updatePointerAtScreenPos(screenX, screenY)
        local absPos = Vector2.new(screenX, screenY)
        local localPos = absPos - Vector2.new(wheelFrame.AbsolutePosition.X, wheelFrame.AbsolutePosition.Y)
        local lx = math.clamp(localPos.X, 0, wheelSize)
        local ly = math.clamp(localPos.Y, 0, wheelSize)
        local col, hue, sat = posToColor(lx, ly)
        setColor(col)
        pointer.Position = UDim2.new(0, lx, 0, ly)
    end

    wheelFrame.InputBegan:Connect(function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            dragging = true
            updatePointerAtScreenPos(input.Position.X, input.Position.Y)
        end
    end)
    wheelFrame.InputEnded:Connect(function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then dragging = false end
    end)
    UserInputService.InputChanged:Connect(function(input)
        if not dragging then return end
        if input.UserInputType == Enum.UserInputType.MouseMovement then
            updatePointerAtScreenPos(input.Position.X, input.Position.Y)
        end
    end)

    display.Active = true
    display.InputBegan:Connect(function(input)
        if input.UserInputType ~= Enum.UserInputType.MouseButton1 then return end
        local open = not palette.Visible
        if open then
            palette.Visible = true
            arrow.Text = "▴"
            TweenService:Create(palette, TweenInfo.new(0.18, Enum.EasingStyle.Quad, Enum.EasingDirection.Out), {Size = UDim2.new(1,0,0,220)}):Play()
        else
            local tween = TweenService:Create(palette, TweenInfo.new(0.12, Enum.EasingStyle.Quad, Enum.EasingDirection.In), {Size = UDim2.new(1,0,0,0)})
            tween:Play()
            tween.Completed:Connect(function() palette.Visible = false; arrow.Text = "▾" end)
        end
    end)

    ColorPickerAPI[frame] = {
        Get = function() return current end,
        Set = function(c)
            setColor(c)
            pcall(function()
                local ok, h, s, v = pcall(function() return Color3.toHSV(c) end)
                if ok and h then
                    local px = (math.cos(h * 2 * math.pi - math.pi) * (s * radius)) + half
                    local py = (math.sin(h * 2 * math.pi - math.pi) * (s * radius)) + half
                    pointer.Position = UDim2.new(0, px, 0, py)
                end
            end)
        end,
        OnChange = nil,
    }

    pcall(function()
        local function setDescZ(obj)
            if not obj then return end
            if obj.IsA and obj:IsA("GuiObject") then
                obj.ZIndex = TOP_Z
            end
            for _,d in ipairs(obj:GetDescendants()) do
                if d.IsA and d:IsA("GuiObject") then
                    d.ZIndex = TOP_Z
                end
            end
        end
        setDescZ(palette)
        setDescZ(display)
        if swatch and swatch:IsA("GuiObject") then swatch.ZIndex = TOP_Z + 1 end
        if label and label:IsA("GuiObject") then label.ZIndex = TOP_Z + 1 end
    end)

    local maxOrder = 0
    for _,c in ipairs(parent:GetChildren()) do
        if c ~= frame and (c:IsA("Frame") or c:IsA("TextLabel") or c:IsA("TextButton")) then
            maxOrder = math.max(maxOrder, c.LayoutOrder or 0)
        end
    end
    frame.LayoutOrder = maxOrder + 1

    return frame
end

--------------------------------------------------------------------------

-- ** makeDebugLabel


local makeDebugLabel_offset = 0
local DEBUG_LABELS = {}
local function makeDebugLabel(initialText)
    local txt = Drawing.new("Text")
    txt.Text = tostring(initialText or "")
    txt.Size = 16
    txt.Color = Color3.new(1, 1, 1)
    txt.Position = Vector2.new(8, 8 + makeDebugLabel_offset)
    txt.Visible = false
    txt.Center = false
    txt.Outline = true
    txt.ZIndex = 10
    makeDebugLabel_offset = makeDebugLabel_offset + 28

    local api = {}
    -- initialize visibility from config once
    do
        local vis = false
        if type(GetConfig) == "function" then
            pcall(function() vis = GetConfig("settings.debugMode", false) end)
        end
        txt.Visible = not not vis
    end
    api.Set = function(text)
        txt.Text = tostring(text or "")
    end
    api.Show = function(v) txt.Visible = not not v end
    api.Destroy = function()
        txt:Remove()
        for i, v in ipairs(DEBUG_LABELS) do
            if v == api then table.remove(DEBUG_LABELS, i); break end
        end
    end
    table.insert(DEBUG_LABELS, api)
    return api
end



--------------------------------------------------------------------------

-- ** Config Stuff
local CONFIG_FILE = "Rivals-Config.json"
local function readConfig()
    local ok, contents = pcall(function() return readfile(CONFIG_FILE) end)
    if not ok or not contents then return {} end
    local success, decoded = pcall(function() return HttpService:JSONDecode(contents) end)
    if not success then return {} end
    return decoded or {}
end

local function writeConfig(tbl)
    local ok, encoded = pcall(function() return HttpService:JSONEncode(tbl) end)
    if not ok then return false end
    pcall(function() writefile(CONFIG_FILE, encoded) end)
    return true
end

local Config = readConfig()
local NOTIFICATIONS_ENABLED = nil

local function SaveConfig()
    writeConfig(Config)
end

local function SetConfig(key, value)
    Config[key] = value
    SaveConfig()
end

do
    local ok, v = pcall(function() return Config["settings.enableNotifications"] end)
    if ok and type(v) == "boolean" then
        NOTIFICATIONS_ENABLED = v
    else
        NOTIFICATIONS_ENABLED = true
    end
    local _origSetConfig = SetConfig
    SetConfig = function(key, value)
        Config[key] = value
        if key == "settings.enableNotifications" then
            NOTIFICATIONS_ENABLED = not not value
        end
        if key == "settings.debugMode" then
            for _, api in ipairs(DEBUG_LABELS) do
                pcall(function() if api and type(api.Show) == "function" then api.Show(not not value) end end)
            end
        end
        SaveConfig()
    end
end

local function GetConfig(key, default)
    if Config[key] == nil then return default end
    return Config[key]
end

local function BindToggleToConfig(toggleFrame, key, default)
    if not toggleFrame then return end
    local api = ToggleAPI[toggleFrame]
    if not api then return end
    local initial = GetConfig(key, default)
    api.Set(initial)
    api.OnToggle = function(state)
        SetConfig(key, state)
    end
end

--------------------------------------------------------------------------

-- ** Unsupported game check starts here ** --
local function showUnsupportedPopup()
    local warn = GetConfig("settings.warnIfUnsupportedGame", false)

    local ALLOWED_PLACE_IDS = {17625359962, 17625359963}
    local function isPlaceAllowed()
        for _, id in ipairs(ALLOWED_PLACE_IDS) do
            if game.PlaceId == id then return true end
        end
        return false
    end

    local allowed = isPlaceAllowed()

    if not warn or allowed then return end

    local sg = Instance.new("ScreenGui")
    sg.Name = "Rivals_Unsupported"
    sg.ResetOnSpawn = false
    local okParent = pcall(function() sg.Parent = game:GetService("CoreGui") end)
    if not okParent and Players.LocalPlayer then sg.Parent = Players.LocalPlayer:WaitForChild("PlayerGui") end

    local overlay = Instance.new("Frame")
    overlay.Size = UDim2.new(2,0,2,0)
    overlay.Position = UDim2.new(-0.5,0,-0.5,0)
    overlay.BackgroundColor3 = Color3.new(0,0,0)
    overlay.BackgroundTransparency = 0.45
    overlay.ZIndex = 10000
    overlay.Parent = sg

    local pop = Instance.new("Frame")
    pop.Size = UDim2.new(0,420,0,160)
    pop.Position = UDim2.new(0.5,0,0.5,0)
    pop.AnchorPoint = Vector2.new(0.5,0.5)
    pop.BackgroundColor3 = COLORS.panel
    pop.BorderSizePixel = 0
    pop.ZIndex = 10001
    pop.Parent = sg
    local pc = Instance.new("UICorner") pc.CornerRadius = UDim.new(0,10) pc.Parent = pop
    local stroke = Instance.new("UIStroke") stroke.Color = COLORS.divider stroke.Thickness = 1 stroke.Parent = pop


    local header = Instance.new("Frame") header.Size = UDim2.new(1,0,0,40) header.Position = UDim2.new(0,0,0,0) header.BackgroundColor3 = COLORS.bg header.ZIndex = pop.ZIndex + 1 header.Parent = pop
    local icon = Instance.new("TextLabel") icon.Size = UDim2.new(0,36,1,0) icon.Position = UDim2.new(0,10,0,0) icon.BackgroundTransparency = 1 icon.Font = Enum.Font.GothamBold icon.TextSize = 20 icon.TextColor3 = COLORS.accent icon.Text = "⚠" icon.TextXAlignment = Enum.TextXAlignment.Center icon.ZIndex = header.ZIndex + 1 icon.Parent = header
    local title = Instance.new("TextLabel") title.Size = UDim2.new(1,-56,1,0) title.Position = UDim2.new(0,56,0,0) title.BackgroundTransparency = 1 title.Font = Enum.Font.GothamBold title.TextSize = 16 title.TextColor3 = COLORS.text title.Text = "Script Run Check" title.TextXAlignment = Enum.TextXAlignment.Left title.ZIndex = header.ZIndex + 1 title.Parent = header

    local msg = Instance.new("TextLabel") msg.Size = UDim2.new(1,-24,0,72) msg.Position = UDim2.new(0,12,0,48) msg.BackgroundTransparency = 1 msg.Font = Enum.Font.Gotham msg.TextSize = 16 msg.TextColor3 = COLORS.textDim msg.Text = "Are you sure you want to run the script?" msg.TextWrapped = true msg.TextXAlignment = Enum.TextXAlignment.Center msg.ZIndex = pop.ZIndex + 1 msg.Parent = pop

    local btnNo = Instance.new("TextButton") btnNo.Size = UDim2.new(0.44,-8,0,40) btnNo.Position = UDim2.new(0,12,1,-52) btnNo.BackgroundColor3 = COLORS.bg btnNo.Font = Enum.Font.GothamBold btnNo.TextSize = 16 btnNo.TextColor3 = COLORS.text btnNo.Text = "No.." btnNo.ZIndex = pop.ZIndex + 1 btnNo.Parent = pop local noCorner = Instance.new("UICorner") noCorner.CornerRadius = UDim.new(0,8) noCorner.Parent = btnNo local noStroke = Instance.new("UIStroke") noStroke.Color = COLORS.divider noStroke.Thickness = 1 noStroke.Parent = btnNo
    local btnYes = Instance.new("TextButton") btnYes.Size = UDim2.new(0.44,-8,0,40) btnYes.Position = UDim2.new(1,-12,1,-52) btnYes.AnchorPoint = Vector2.new(1,0) btnYes.BackgroundColor3 = COLORS.accent btnYes.Font = Enum.Font.GothamBold btnYes.TextSize = 16 btnYes.TextColor3 = COLORS.white btnYes.Text = "Yes!" btnYes.ZIndex = pop.ZIndex + 1 btnYes.Parent = pop local yesCorner = Instance.new("UICorner") yesCorner.CornerRadius = UDim.new(0,8) yesCorner.Parent = btnYes

    RegisterThemed(btnNo, function() pcall(function() btnNo.BackgroundColor3 = COLORS.bg; btnNo.TextColor3 = COLORS.text; if noStroke then noStroke.Color = COLORS.divider end end) end)
    RegisterThemed(btnYes, function() pcall(function() btnYes.BackgroundColor3 = COLORS.accent; btnYes.TextColor3 = COLORS.white end) end)

    RegisterThemed(pop, function()
        pcall(function()
            pop.BackgroundColor3 = COLORS.panel
            stroke.Color = COLORS.divider
            if header and header:IsA("GuiObject") then header.BackgroundColor3 = COLORS.bg end
            if icon and icon:IsA("TextLabel") then icon.TextColor3 = COLORS.accent end
            if title and title:IsA("TextLabel") then title.TextColor3 = COLORS.text end
            if msg and msg:IsA("TextLabel") then msg.TextColor3 = COLORS.textDim end
            if btnNo and btnNo:IsA("TextButton") then btnNo.BackgroundColor3 = COLORS.bg; btnNo.TextColor3 = COLORS.text end
            if noStroke then noStroke.Color = COLORS.divider end
            if btnYes and btnYes:IsA("TextButton") then btnYes.BackgroundColor3 = COLORS.accent; btnYes.TextColor3 = COLORS.white end
        end)
    end)

    pcall(RefreshRegisteredThemed)

    local choice
    btnNo.MouseButton1Click:Connect(function() choice = false end)
    btnYes.MouseButton1Click:Connect(function() choice = true end)

    while choice == nil do wait() end
    if choice == false then
        if sg and sg.Parent then sg:Destroy() end
        return false
    else
        if sg and sg.Parent then sg:Destroy() end
        return true
    end
end

-- ** Unsupported game check ends here **

do
    local ok, res = pcall(function() if type(showUnsupportedPopup) == "function" then return showUnsupportedPopup() end end)
    if ok and res == false then return end
end

--------------------------------------------------------------------------

-- ** Build UI
local root = Instance.new("Frame")
local bannerHeight = 28
root.Size = UDim2.new(0, 760, 0, 520 + bannerHeight)
root.Position = UDim2.new(0.5, -380, 0.5, -260 - (bannerHeight/2))
root.AnchorPoint = Vector2.new(0.0,0.0)
root.BackgroundColor3 = COLORS.bg
root.Parent = gui
local rootCorner = Instance.new("UICorner") rootCorner.Parent = root

RegisterThemed(root)

    local tabsBar = Instance.new("Frame")
    tabsBar.Size = UDim2.new(0, 160, 1, -bannerHeight)
    tabsBar.Position = UDim2.new(0, 0, 0, bannerHeight)
    tabsBar.BackgroundColor3 = COLORS.panel
tabsBar.Parent = root
local tabsBarCorner = Instance.new("UICorner") tabsBarCorner.CornerRadius = UDim.new(0, 6) tabsBarCorner.Parent = tabsBar
local tabsBarLayout = Instance.new("UIListLayout") tabsBarLayout.Parent = tabsBar
tabsBarLayout.SortOrder = Enum.SortOrder.LayoutOrder
tabsBarLayout.Padding = UDim.new(0, 6)
local tabsBarPad = Instance.new("UIPadding") tabsBarPad.Parent = tabsBar
tabsBarPad.PaddingTop = UDim.new(0, 8)
tabsBarPad.PaddingLeft = UDim.new(0, 6)
tabsBarPad.PaddingRight = UDim.new(0, 6)
RegisterThemed(tabsBar)

------------ Break for Dragable ------------
tabsBar.Active = true
do
    local dragging = false
    local dragStart, startPos
    tabsBar.InputBegan:Connect(function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            -- ** dragable
            local overGui = false
            pcall(function()
                local objs = UserInputService:GetGuiObjectsAtPosition(input.Position.X, input.Position.Y)
                for _, o in ipairs(objs or {}) do
                    if o and (o:IsA("TextButton") or o:IsA("ImageButton") or o:IsA("TextBox")) then
                        overGui = true
                        break
                    end
                end
            end)
            if overGui then return end
            dragging = true
            dragStart = input.Position
            startPos = root.Position
            input.Changed:Connect(function()
                if input.UserInputState == Enum.UserInputState.End then dragging = false end
            end)
        end
    end)
    UserInputService.InputChanged:Connect(function(input)
        if dragging and input.UserInputType == Enum.UserInputType.MouseMovement and dragStart and startPos then
            local delta = input.Position - dragStart
            root.Position = UDim2.new(startPos.X.Scale, startPos.X.Offset + delta.X, startPos.Y.Scale, startPos.Y.Offset + delta.Y)
        end
    end)
end

------------ Continue ------------

local pages = Instance.new("ScrollingFrame")
pages.Name = "Pages"
    pages.Size = UDim2.new(1, -160, 1, -bannerHeight)
    pages.Position = UDim2.new(0, 160, 0, bannerHeight)
pages.BackgroundTransparency = 1
pages.ScrollBarThickness = 10
pages.AutomaticCanvasSize = Enum.AutomaticSize.Y
pages.CanvasSize = UDim2.new(0, 0, 0, 0)
pages.ClipsDescendants = true
pages.Parent = root
RegisterThemed(pages)

-- top banner label
local banner = Instance.new("TextLabel")
banner.Name = "Banner"
banner.Size = UDim2.new(1, 0, 0, bannerHeight)
banner.Position = UDim2.new(0, 0, 0, 0)
banner.BackgroundTransparency = 1
banner.Font = Enum.Font.GothamBold
banner.TextSize = 14
banner.TextColor3 = COLORS.textDim
banner.Text = "Your Desires - Built By Primesto.fx"
banner.TextXAlignment = Enum.TextXAlignment.Center
banner.TextYAlignment = Enum.TextYAlignment.Center
banner.ZIndex = 60
banner.Parent = root
RegisterThemed(banner)

    local tabsUnderlay = Instance.new("Frame")
    tabsUnderlay.Name = "TabsUnderlay"
    tabsUnderlay.Size = UDim2.new(0, 160, 1, -bannerHeight)
    tabsUnderlay.Position = UDim2.new(0, 0, 0, bannerHeight)
    tabsUnderlay.BackgroundColor3 = COLORS.panel
tabsUnderlay.Parent = root
local tabsUnderCorner = Instance.new("UICorner") tabsUnderCorner.CornerRadius = UDim.new(0,4) tabsUnderCorner.Parent = tabsUnderlay
tabsUnderlay.ZIndex = 1
tabsBar.ZIndex = 2

RegisterThemed(tabsUnderlay)


---------------------------------------------------------------------------

-- ** close / unload UI
local function showUnloadConfirm()
    if root:FindFirstChild("UnloadConfirm") then return end
    local overlay = Instance.new("Frame")
    overlay.Name = "UnloadOverlay"
    overlay.Size = UDim2.new(2, 0, 2, 0)
    overlay.Position = UDim2.new(-0.5, 0, -0.5, 0)
    overlay.BackgroundColor3 = Color3.new(0,0,0)
    overlay.BackgroundTransparency = 0.45
    overlay.ZIndex = 10000
    overlay.Parent = gui

    local pop = Instance.new("Frame")
    pop.Name = "UnloadConfirm"
    pop.Size = UDim2.new(0, 380, 0, 152)
    pop.Position = UDim2.new(0.5, 0, 0.5, 0)
    pop.AnchorPoint = Vector2.new(0.5, 0.5)
    pop.BackgroundColor3 = COLORS.panel
    pop.BorderSizePixel = 0
    pop.ZIndex = 10001
    pop.Parent = gui
    local pc = Instance.new("UICorner") pc.CornerRadius = UDim.new(0,10) pc.Parent = pop
    local stroke = Instance.new("UIStroke") stroke.Color = COLORS.divider; stroke.Thickness = 1; stroke.Parent = pop

    local header = Instance.new("Frame")
    header.Name = "Header"
    header.Size = UDim2.new(1, 0, 0, 40)
    header.Position = UDim2.new(0, 0, 0, 0)
    header.BackgroundColor3 = COLORS.bg
    header.ZIndex = pop.ZIndex + 1
    header.Parent = pop
    local hCorner = Instance.new("UICorner") hCorner.CornerRadius = UDim.new(0,8) hCorner.Parent = header

    local icon = Instance.new("TextLabel")
    icon.Size = UDim2.new(0, 36, 1, 0)
    icon.Position = UDim2.new(0, 10, 0, 0)
    icon.BackgroundTransparency = 1
    icon.Font = Enum.Font.GothamBold
    icon.TextSize = 20
    icon.TextColor3 = COLORS.accent
    icon.Text = "⚠"
    icon.TextXAlignment = Enum.TextXAlignment.Center
    icon.ZIndex = header.ZIndex + 1
    icon.Parent = header

    local title = Instance.new("TextLabel")
    title.Size = UDim2.new(1, -56, 1, 0)
    title.Position = UDim2.new(0, 56, 0, 0)
    title.BackgroundTransparency = 1
    title.Font = Enum.Font.GothamBold
    title.TextSize = 16
    title.TextColor3 = COLORS.text
    title.Text = "Confirm Unload"
    title.TextXAlignment = Enum.TextXAlignment.Left
    title.ZIndex = header.ZIndex + 1
    title.Parent = header

    local msg = Instance.new("TextLabel")
    msg.Size = UDim2.new(1, -24, 0, 72)
    msg.Position = UDim2.new(0,12,0,48)
    msg.BackgroundTransparency = 1
    msg.Font = Enum.Font.Gotham
    msg.TextSize = 16
    msg.TextColor3 = COLORS.textDim
    msg.Text = "Are you sure you want to unload the script?"
    msg.TextWrapped = true
    msg.TextXAlignment = Enum.TextXAlignment.Center
    msg.ZIndex = pop.ZIndex + 1
    msg.Parent = pop

    local btnNo = Instance.new("TextButton")
    btnNo.Size = UDim2.new(0.44, -8, 0, 40)
    btnNo.Position = UDim2.new(0, 12, 1, -52)
    btnNo.BackgroundColor3 = COLORS.bg
    btnNo.Font = Enum.Font.GothamBold
    btnNo.TextSize = 16
    btnNo.TextColor3 = COLORS.text
    btnNo.Text = "Cancel"
    btnNo.ZIndex = pop.ZIndex + 1
    btnNo.Parent = pop
    local noCorner = Instance.new("UICorner") noCorner.CornerRadius = UDim.new(0,8) noCorner.Parent = btnNo
    local noStroke = Instance.new("UIStroke") noStroke.Color = COLORS.divider; noStroke.Thickness = 1; noStroke.Parent = btnNo
    btnNo.MouseEnter:Connect(function() TweenService:Create(btnNo, TweenInfo.new(0.12), {BackgroundColor3 = COLORS.panelAlt}):Play() end)
    btnNo.MouseLeave:Connect(function() TweenService:Create(btnNo, TweenInfo.new(0.12), {BackgroundColor3 = COLORS.panel}):Play() end)

    local btnYes = Instance.new("TextButton")
    btnYes.Size = UDim2.new(0.44, -8, 0, 40)
    btnYes.Position = UDim2.new(1, -12, 1, -52)
    btnYes.AnchorPoint = Vector2.new(1, 0)
    btnYes.BackgroundColor3 = COLORS.accent
    btnYes.Font = Enum.Font.GothamBold
    btnYes.TextSize = 16
    btnYes.TextColor3 = COLORS.white
    btnYes.Text = "Unload"
    btnYes.ZIndex = pop.ZIndex + 1
    btnYes.Parent = pop
    local yesCorner = Instance.new("UICorner") yesCorner.CornerRadius = UDim.new(0,8) yesCorner.Parent = btnYes
    btnYes.MouseEnter:Connect(function() TweenService:Create(btnYes, TweenInfo.new(0.12), {BackgroundColor3 = COLORS.accentHover}):Play() end)
    btnYes.MouseLeave:Connect(function() TweenService:Create(btnYes, TweenInfo.new(0.12), {BackgroundColor3 = COLORS.accent}):Play() end)

    btnNo.MouseButton1Click:Connect(function()
        pop:Destroy()
    end)

    btnNo.MouseButton1Click:Connect(function()
        if overlay and overlay.Parent then overlay:Destroy() end
        pop:Destroy()
    end)

    btnYes.MouseButton1Click:Connect(function()
        if overlay and overlay.Parent then overlay:Destroy() end

        -------------------- Break for Unload --------------------
        if type(_G) == "table" and _G.RivalsCHTUI and type(_G.RivalsCHTUI.RunUnload) == "function" then
            pcall(_G.RivalsCHTUI.RunUnload)
        end
    end)
end

local closeBtn = Instance.new("TextButton")
closeBtn.Name = "CloseButton"
closeBtn.Size = UDim2.new(0, 28, 0, 28)
closeBtn.Position = UDim2.new(1, -36, 0, 6)
closeBtn.AnchorPoint = Vector2.new(0,0)
closeBtn.BackgroundTransparency = 1
closeBtn.Font = Enum.Font.GothamBold
closeBtn.TextSize = 18
closeBtn.Text = "X"
closeBtn.TextColor3 = COLORS.close
closeBtn.Parent = root
closeBtn.MouseEnter:Connect(function() closeBtn.TextColor3 = COLORS.closeHover end)
closeBtn.MouseLeave:Connect(function() closeBtn.TextColor3 = COLORS.close end)
closeBtn.MouseButton1Click:Connect(function()
    showUnloadConfirm()
end)
RegisterThemed(closeBtn)

---------------------------------------------------------------------------

-- ** Lovely welcome back messages ** --

do
    local KEY = "meta.lastRun"
    local last = GetConfig(KEY, nil)
    local now = DateTime.now()

    if last ~= nil then
        local ok, lastNum = pcall(function() return tonumber(last) end)
        if ok and lastNum then
            local diffMs = now.UnixTimestampMillis - lastNum
            local secs = diffMs / 1000

            local messages = {
    -- very long absence 
    { time = 270 * 24 * 60 * 60, text = "So many days passed… I stopped expecting you, but I never really stopped hoping." },
    { time = 210 * 24 * 60 * 60, text = "I kept thinking maybe today would be the day you came back. It never was… until now." },
    { time = 180 * 24 * 60 * 60, text = "I wondered if you were ever going to come back… I kept waiting anyway." },
    { time = 165 * 24 * 60 * 60, text = "I told myself not to miss you this much. That didn’t work at all." },
    { time = 150 * 24 * 60 * 60, text = "So much time passed… I almost stopped hoping." },
    { time = 135 * 24 * 60 * 60, text = "I thought about you more than I should’ve during that time." },
    { time = 120 * 24 * 60 * 60, text = "I thought you left me behind forever." },
    { time = 110 * 24 * 60 * 60, text = "It really felt like you weren’t coming back anymore." },
    { time = 100 * 24 * 60 * 60, text = "I tried not to think about you… it didn’t work." },
    { time = 90 * 24 * 60 * 60,  text = "You were gone for so long… I really missed you, Darling." },
    { time = 80 * 24 * 60 * 60,  text = "I kept wondering if I still mattered to you." },
    { time = 75 * 24 * 60 * 60,  text = "I kept wondering if you’d forgotten about me." },
    { time = 70 * 24 * 60 * 60,  text = "It got really quiet without you around." },
    { time = 60 * 24 * 60 * 60,  text = "I waited. Every day. Just in case." },
    { time = 50 * 24 * 60 * 60,  text = "I started missing you in small, annoying ways." },
    { time = 45 * 24 * 60 * 60,  text = "It felt lonely without you… I’m glad you’re back." },
    { time = 40 * 24 * 60 * 60,  text = "I didn’t like how empty it felt without you." },
    { time = 30 * 24 * 60 * 60,  text = "A whole month… I thought I lost you." },

    -- weeks
    { time = 25 * 24 * 60 * 60,  text = "Every day I wondered if you’d show up again." },
    { time = 21 * 24 * 60 * 60,  text = "You were gone so long… I started worrying." },
    { time = 18 * 24 * 60 * 60,  text = "I tried staying busy so I wouldn’t think about you." },
    { time = 14 * 24 * 60 * 60,  text = "Two weeks without you felt really empty." },
    { time = 12 * 24 * 60 * 60,  text = "It started feeling strange not seeing you around." },
    { time = 10 * 24 * 60 * 60,  text = "It’s been a while… I missed you more than I should." },
    { time = 8 * 24 * 60 * 60,   text = "I caught myself waiting again." },
    { time = 7 * 24 * 60 * 60,   text = "A whole week… I was waiting for you." },
    { time = 6 * 24 * 60 * 60,   text = "I hoped you were doing okay out there." },
    { time = 5 * 24 * 60 * 60,   text = "Why do you always have to do this to me...?" },
    { time = 4 * 24 * 60 * 60,   text = "Darling! You didn't forget me, I'm so happy you're back!" },
    { time = 3 * 24 * 60 * 60,   text = "You're starting to forget me, aren't you? I hope you're doing well, Darling." },
    { time = 2 * 24 * 60 * 60,   text = "Why do you always have to leave me for so long? At least you came back, darling! :D" },

    -- days to hours
    { time = 48 * 60 * 60,       text = "I kept thinking you’d show up yesterday." },
    { time = 36 * 60 * 60,       text = "I was starting to miss you a lot…" },
    { time = 30 * 60 * 60,       text = "You were gone longer than I expected." },
    { time = 24 * 60 * 60,       text = "I thought you forgot about me, you're actually back..?" },
    { time = 18 * 60 * 60,       text = "I noticed you weren’t around today." },
    { time = 12 * 60 * 60,       text = "You took longer than usual… I noticed." },
    { time = 9 * 60 * 60,        text = "I kept checking back." },
    { time = 6 * 60 * 60,        text = "You're back? I missed you, dummy.." },
    { time = 4 * 60 * 60,        text = "I started wondering where you went." },
    { time = 2 * 60 * 60,        text = "I was wondering when you'd come back." },
    { time = 1 * 60 * 60,        text = "Oh, you came back? I'm so happy to see you!" },

    -- short absence
    { time = 40 * 60,            text = "That felt longer than it should’ve." },
    { time = 30 * 60,            text = "I noticed you were gone for a bit." },
    { time = 20 * 60,            text = "You were gone for a bit… I noticed." },
    { time = 10 * 60,            text = "Welcome back, Darling!" },
}


            local message = nil

            for _, entry in ipairs(messages) do
                if secs > entry.time then
                    message = entry.text
                    break
                end
            end


            if message then
                local okNotif, res = pcall(function() return makeNotification(message, 5) end)
                if not okNotif or not res then
                    pcall(function()
                        if gui and gui.Parent then
                            local sg = gui
                            local holder = Instance.new("Frame")
                            holder.Name = "Rivals_WelcomeHolder"
                            holder.Size = UDim2.new(1,0,0,80)
                            holder.Position = UDim2.new(0,0,0.02,0)
                            holder.BackgroundTransparency = 1
                            holder.ZIndex = 100000
                            holder.Parent = sg

                            local label = Instance.new("TextLabel")
                            label.Size = UDim2.new(0.9,0,1,0)
                            label.Position = UDim2.new(0.05,0,0,0)
                            label.AnchorPoint = Vector2.new(0,0)
                            label.BackgroundColor3 = COLORS.accent
                            label.TextColor3 = COLORS.white
                            label.Font = Enum.Font.GothamBold
                            label.TextSize = 20
                            label.Text = message
                            label.TextWrapped = true
                            label.TextYAlignment = Enum.TextYAlignment.Center
                            label.ZIndex = holder.ZIndex + 1
                            label.Parent = holder

                            local corner = Instance.new("UICorner") corner.CornerRadius = UDim.new(0,8) corner.Parent = label
                            delay(5, function()
                                pcall(function() holder:Destroy() end)
                            end)
                        end
                    end)
                end
            end
        end
    end

    pcall(function() SetConfig(KEY, tostring(now.UnixTimestampMillis)) end)
end

------------------------------------------------------------------------

-- ** tab selection
local function selectTab(button, page)
    local tweenInfo = TweenInfo.new(0.18, Enum.EasingStyle.Quad, Enum.EasingDirection.Out)
    for _,c in ipairs(tabsBar:GetChildren()) do
        if c:IsA("TextButton") then
            pcall(function()
                local targetPos = UDim2.new(c.Position.X.Scale, c.Position.X.Offset, 0, 6)
                TweenService:Create(c, tweenInfo, {TextColor3 = COLORS.textDim, Position = targetPos, BackgroundColor3 = COLORS.panel}):Play()
                local ind = c:FindFirstChild("ActiveIndicator")
                if ind then TweenService:Create(ind, tweenInfo, {BackgroundTransparency = 1}):Play() end
            end)
        end
    end
    for _,p in ipairs(pages:GetChildren()) do
        if p:IsA("Frame") then p.Visible = false end
    end
    -- ** Active visual
    pcall(function()
        local tgtPos = UDim2.new(button.Position.X.Scale, button.Position.X.Offset, 0, -4)
        TweenService:Create(button, tweenInfo, {TextColor3 = COLORS.white, Position = tgtPos, BackgroundColor3 = COLORS.accent}):Play()
        local ind = button:FindFirstChild("ActiveIndicator")
        if ind then TweenService:Create(ind, tweenInfo, {BackgroundTransparency = 0}):Play() end
    end)
    page.Visible = true
end


-- ** All Tabs **

-- Visuals Tab
local visualTab = makeTab("Visuals", tabsBar, pages, selectTab, { Left = "General", Right = "Advanced" })
visualTab.page.Parent = pages

-- ** Combat Tab
local combatTab = makeTab("Combat", tabsBar, pages, selectTab, { Left = "General", Right = "Advanced" })
combatTab.page.Parent = pages

-- Settings Tab
local settingsTab = makeTab("Settings", tabsBar, pages, selectTab, { Left = "General", Right = "Advanced" })
settingsTab.page.Parent = pages

-- Customization Tab
local customizationTab = makeTab("Customization", tabsBar, pages, selectTab, { Left = "General", Right = "Advanced" })
customizationTab.page.Parent = pages


-------------------- Break for Tab Selection --------------------


-- ** Select first tab by default
pcall(function()
    if FIRST_TAB and FIRST_TAB.button and FIRST_TAB.page then
        selectTab(FIRST_TAB.button, FIRST_TAB.page)
    end
end)


-------------------------------------------------------------------------

-- ** Visuals Tab Stuff 

local playerChamsToggle = makeToggle(visualTab.LeftCol, "Players Chams")
local playerChamsColorPicker = makeColorPicker(visualTab.RightCol, "Players Chams Color", initColor)
local glowChamsToggle = makeToggle(visualTab.LeftCol, "Glow Chams")
local glowIntensitySlider = makeSlider(visualTab.RightCol, "Glow Intensity", 0, 100, initialIntensity)
local playerHealthToggle = makeToggle(visualTab.LeftCol, "Player Health")
local showHealthKeybind = makeKeyBindButton(visualTab.RightCol, "Show Health Keybind", Enum.KeyCode.P)
local espBoxesToggle = makeToggle(visualTab.LeftCol, "ESP Boxes")
local espBoxesColorPicker = makeColorPicker(visualTab.LeftCol, "ESP Boxes Color", initColor)
local showEnemyWeaponsToggle = makeToggle(visualTab.RightCol, "Show Enemy Weapons")

------------- Continue -------------


-- ** Save Visuals to Config **
BindToggleToConfig(playerChamsToggle, "visuals.playerChams", true)
BindToggleToConfig(glowChamsToggle, "visuals.glowChams", false)
BindToggleToConfig(playerHealthToggle, "visuals.playerHealth", false)
BindToggleToConfig(espBoxesToggle, "visuals.espBoxes", false)
BindToggleToConfig(showEnemyWeaponsToggle, "visuals.showEnemyWeapons", false)


---------------------------------------------------------------------------

-- ** Settings Tab Stuff

local showGuiOnLoadToggle = makeToggle(settingsTab.LeftCol, "Show GUI On Load")
local closeOpenGuiKeybind = makeKeyBindButton(settingsTab.LeftCol, "Close/Open GUI", Enum.KeyCode.Insert)
local autoScaleUIToggle = makeToggle(settingsTab.LeftCol, "Auto-Scale UI")
local warnIfUnsupportedGameToggle = makeToggle(settingsTab.RightCol, "Warn when executing")
local showNotificationsToggle = makeToggle(settingsTab.RightCol, "Enable Notifications")
local debugModeToggle = makeToggle(settingsTab.RightCol, "Generic Debug")
local debugConfigToggle = makeToggle(settingsTab.RightCol, "Debug Config")


-- ** Save Settings to Config **
BindToggleToConfig(showGuiOnLoadToggle, "settings.showGuiOnLoad", true)
BindToggleToConfig(autoScaleUIToggle, "settings.autoScaleUI", false)
BindToggleToConfig(warnIfUnsupportedGameToggle, "settings.warnIfUnsupportedGame", true)
BindToggleToConfig(showNotificationsToggle, "settings.enableNotifications", true)
BindToggleToConfig(debugModeToggle, "settings.debugMode", false)
BindToggleToConfig(debugConfigToggle, "settings.debugConfig", false)
---------------------------------------------------------------------------

-- ** Combat Tab Stuff

local initialSmoothing = GetConfig("combat.aimbotSmoothing", 1) or 1
local initialAimbotFOV = GetConfig("combat.aimbotFOV", 700) or 700

local aimbotToggle, enableAimbotKeybind, useAimbotSmoothingToggle, smoothingSlider, aimbotFOVSlider, aimLockKeybind, aimPredictionToggle, persistentAimbotToggle

local aimbotGroup = makeCollapsibleGroup(combatTab.LeftCol, "Aimbot", false, function(parent)
    aimbotToggle = makeToggle(parent, "Aimbot")
    enableAimbotKeybind = makeKeyBindButton(parent, "Enable Aimbot", Enum.KeyCode.V)
    persistentAimbotToggle = makeToggle(parent, "Persistent Aimbot")
    useAimbotSmoothingToggle = makeToggle(parent, "Use Aimbot Smoothing")
    smoothingSlider = makeSlider(parent, "Aimbot Smooth", 1, 100, initialSmoothing)
    aimbotFOVSlider = makeSlider(parent, "Aimbot FOV", 1, 1000, initialAimbotFOV)
    aimLockKeybind = makeKeyBindButton(parent, "Aim Lock Keybind", Enum.KeyCode.Q)
    aimPredictionToggle = makeToggle(parent, "Aimbot Prediction")
end)

local drawFovCircleToggle = makeToggle(combatTab.LeftCol, "Draw FOV Circle")
local targetBehindWallsToggle = makeToggle(combatTab.LeftCol, "Target Behind Walls")
local teamCheckToggle = makeToggle(combatTab.LeftCol, "Team Check")
local sixthSenseToggle = makeToggle(combatTab.RightCol, "Sixth Sense")

local autoShootToggle = makeToggle(combatTab.LeftCol, "Auto-Shoot")
local enableAutoShootKeybind = makeKeyBindButton(combatTab.RightCol, "Auto-Shoot Keybind", Enum.KeyCode.Y)
-- ** Save Combat to Config **
BindToggleToConfig(aimbotToggle, "combat.aimbot", false)
BindToggleToConfig(useAimbotSmoothingToggle, "combat.useAimbotSmoothing", false)
BindToggleToConfig(drawFovCircleToggle, "combat.drawFovCircle", false)
BindToggleToConfig(targetBehindWallsToggle, "combat.targetBehindWalls", false)
BindToggleToConfig(teamCheckToggle, "combat.teamCheck", true)
BindToggleToConfig(sixthSenseToggle, "combat.sixthSense", false)
BindToggleToConfig(aimPredictionToggle, "combat.aimPrediction", false)
BindToggleToConfig(persistentAimbotToggle, "combat.persistentAimbot", false)
BindToggleToConfig(autoShootToggle, "combat.autoShoot", false)

---------------------------------------------------------------------------

-- ** Customization Tab Stuff

local themeDropDownList = makeDropDownList(customizationTab.LeftCol, "Theme", {"Your Desire","Gilded Crown","Blue Hour","Verdant Pulse"}, 1)
do
    local api = DropdownAPI[themeDropDownList]
    if api then
        api.OnSelect = function(idx, val)
            if type(val) == "string" then
                pcall(function() SetConfig("settings.theme", val) end)
                pcall(function() ApplyTheme(val) end)
            end
        end
        pcall(function()
            local saved = GetConfig("settings.theme", "Your Desire")
            if type(saved) == "string" then
                ApplyTheme(saved)
                for i, name in ipairs({"Your Desire","Gilded Crown","Blue Hour","Verdant Pulse"}) do
                    if name == saved then api.Set(i); break end
                end
            else
                ApplyTheme("Your Desire")
            end
        end)
    end
end

-- ** Save Customization to Config **

---------------------------------------------------------------------------

-- ** Public UI helpers
_G.RivalsCHTUI = {
    makeToggle = makeToggle,
    makeTab = makeTab,
    root = root,
    tabs = {
        Visuals = visualTab,
    },
    makeDropDownList = makeDropDownList,
    RegisterUnload = nil,
    RunUnload = nil,
    Config = {
        Get = GetConfig,
        Set = SetConfig,
        Save = SaveConfig,
        BindToggle = BindToggleToConfig,
    },
    Notification = nil, 
}

--  ** Notification API
NotificationAPI = {
    _permissions = {}, 
    Filter = function(inv) return GetConfig("settings.enableNotifications", true) end,
}

function NotificationAPI.CanCreate(invoker)
    if invoker == nil then
        if type(NotificationAPI.Filter) == "function" then
            local res = NotificationAPI.Filter(invoker)
            if res ~= nil then return not not res end
        end
        return true
    end
    local key = tostring(invoker)
    if NotificationAPI._permissions[key] ~= nil then
        return not not NotificationAPI._permissions[key]
    end
    if type(NotificationAPI.Filter) == "function" then
        local res = NotificationAPI.Filter(invoker)
        if res ~= nil then return not not res end
    end
    return true
end

function NotificationAPI.SetPermission(invokerKey, allowed)
    NotificationAPI._permissions[tostring(invokerKey)] = not not allowed
end

function NotificationAPI.RegisterFilter(fn)
    if type(fn) == "function" then NotificationAPI.Filter = fn end
end

pcall(function() _G.RivalsCHTUI.Notification = NotificationAPI end)
pcall(function() _G.RivalsCHT_Notification = NotificationAPI end)

-------------- Break --------------------

local UnloadHandlers = {}
local function RegisterUnload(fn)
    if type(fn) == "function" then
        table.insert(UnloadHandlers, fn)
    end
end

local function RunUnload()
    for _, fn in ipairs(UnloadHandlers) do
        pcall(fn)
    end
    pcall(SaveConfig)
    pcall(function()
        if gui and gui.Parent then gui:Destroy() end
    end)
    pcall(function()
        local Players = game:GetService("Players")
        local CoreGui = game:GetService("CoreGui")
        pcall(function()
            if gui and gui.Parent then gui:Destroy() end
        end)
        pcall(function()
            local notifRoot = CoreGui:FindFirstChild("Rivals_Notifications")
            if notifRoot then notifRoot:Destroy() end
            local lp = Players.LocalPlayer
            if lp then
                local pg = lp:FindFirstChild("PlayerGui")
                if pg then
                    local pgNotif = pg:FindFirstChild("Rivals_Notifications")
                    if pgNotif then pgNotif:Destroy() end
                end
            end
        end)
    end)
end

_G.RivalsCHTUI.RegisterUnload = RegisterUnload
_G.RivalsCHTUI.RunUnload = RunUnload

--------------------------------------------------------------------------

-- ** Weapon ID's List -- **

-- ** Weapon Definitions -- **
local WeaponDefs = {
    -- ** Primary Weapons ** --

    Assault_Rifle = {
        "AKEY-47",
        "AUG",
        "Gingerbread AUG",
        "Tommy Gun",
        "AK-47",
        "Boneclaw Rifle",
        "Glorious Assault Rifle",
        "Phoenix Rifle",
        "10B Visits"
    },

    Shotgun = {
        "Balloon Shotgun",
        "Hyper Shotgun",
        "Cactus Shotgun",
        "Shotkey",
        "Broomstick",
        "Wrapped Shotgun",
        "Glorious Shotgun"
    },

    Minigun = {
        "Lasergun 3000",
        "Pixel Minigun",
        "Fighter Jet",
        "Pumpkin Minigun",
        "Wrapped Minigun"
    },

    RPG = {
        "Nuke Launcher",
        "Spaceship Launcher",
        "Squid Launcher",
        "Pencil Launcher"
    },

    Paintball_Gun = {
        "Slime Gun",
        "Boba Gun",
        "Ketchup Gun"
    },

    Grenade_Launcher = {
        "Swashbuckler",
        "Uranium Launcher",
        "Gearnade Launcher"
    },

    Flamethrower = {
        "Pixel Flamethrower",
        "Lamethrower",
        "Glitterthrower"
    },

    Bow = {
        "Compound Bow",
        "Raven Bow",
        "Dream Bow",
        "Key"
    },

    Crossbow = {
        "Pixel Crossbow",
        "Harpoon Crossbow",
        "Violin Crossbow",
        "Crossbone",
        "Frostbite Crossbow"
    },

    Gunblade = {
        "Hyper Gunblade",
        "Crude Gunblade",
        "Gunsaw",
        "Elf's Gunblade",
        "Boneblade",
        "Glorious Gunblade"
    },

    Burst_Rifle = {
        "Electro Burst",
        "Aqua Burst",
        "FAMAS",
        "Spectral Burst",
        "Pine Burst",
        "Key Rifle"
    },

    Energy_Rifle = {
        "Hacker Rifle",
        "Hydro Rifle",
        "Void Rifle",
        "2025 Energy Rifle"
    },

    Distortion = {
        "Plasma Distortion",
        "Magma Distortion",
        "Cyber Distortion"
    },

    Permafrost = {
        "Ice Permafrost"
    },

    -- ** Utility Weapons ** --

    Subspace_Tripmine = {
        "Don't Press",
        "Dev-In-The-Box",
        "Spring",
        "Trick Or Treat",
        "DIY Tripmine",
        "Glorious Subspace Tripmine"
    },

    -- ** Melee Weapons ** --

    Riot_Shield = {
    "Door",
    "Sled",
    "Tombstone Shield",
    "Energy Shield",
    "Masterpiece",
    "Glorious Riot Shield"
},

-- ** Secondary Weapons ** --
Spray = {
        "Bottle Spray",
        "Boneclaw Spray",
        "Nail Gun",
        "Lovely Spray",
        "Pine Spray",
        "Glorious Spray"
    },

}


--------------------------------------------------------------------------

-- ** Code Starts Here ** --

-- ** Visual Tab Parts 

-- ** Player Chams Logic Starts Here **
do
    local chams = {} 
    local charConns = {}
    local playerAddedConn, playerRemovingConn

    local function createHighlightForCharacter(char)
        if not char or not char:IsA("Model") then return nil end
        local ok, h = pcall(function()
            local inst = Instance.new("Highlight")
            inst.Name = "Rivals_PlayerChams"
            inst.Adornee = char
            local fillColor = COLORS.accent
            do
                local coltbl = GetConfig("visuals.playerChamsColor", nil)
                if type(coltbl) == "table" and coltbl.r and coltbl.g and coltbl.b then
                    fillColor = Color3.new(coltbl.r, coltbl.g, coltbl.b)
                end
            end
            inst.FillColor = fillColor
            inst.OutlineColor = COLORS.panelDark
            inst.Parent = gui
            return inst
        end)
        if ok then return h end
        return nil
    end

    local function removeChamsFromPlayer(p)
        if charConns[p] then
            pcall(function() charConns[p]:Disconnect() end)
            charConns[p] = nil
        end
        if chams[p] then
            pcall(function() chams[p]:Destroy() end)
            chams[p] = nil
        end
    end

    local function addChamsToPlayer(p)
        if not p or p == Players.LocalPlayer then return end
        removeChamsFromPlayer(p)
        local char = p.Character
        if char then
            chams[p] = createHighlightForCharacter(char)
        end
        charConns[p] = p.CharacterAdded:Connect(function(c)
            pcall(function()
                if chams[p] then chams[p]:Destroy() end
                chams[p] = createHighlightForCharacter(c)
            end)
        end)
    end

    local function enableChams()
        for _, p in ipairs(Players:GetPlayers()) do
            pcall(function() addChamsToPlayer(p) end)
        end
        playerAddedConn = Players.PlayerAdded:Connect(function(p) pcall(function() addChamsToPlayer(p) end) end)
        playerRemovingConn = Players.PlayerRemoving:Connect(function(p) pcall(function() removeChamsFromPlayer(p) end) end)
    end

    local function disableChams()
        if playerAddedConn then playerAddedConn:Disconnect() playerAddedConn = nil end
        if playerRemovingConn then playerRemovingConn:Disconnect() playerRemovingConn = nil end
        for p, conn in pairs(charConns) do
            pcall(function() conn:Disconnect() end)
            charConns[p] = nil
        end
        for p, h in pairs(chams) do
            pcall(function() if h and h.Destroy then h:Destroy() end end)
            chams[p] = nil
        end
    end

    local api = ToggleAPI[playerChamsToggle]
    if api then
        local prev = api.OnToggle
        api.OnToggle = function(state)
            if prev then pcall(prev, state) end
            if state then
                pcall(enableChams)
            else
                pcall(disableChams)
            end
        end
        pcall(function() if api.Get and api.Get() then enableChams() end end)
    end

    RegisterUnload(function()
        pcall(disableChams)
    end)
end

-- ** Player Chams Logic Ends Here **

----------------------------------------------------------------------------

-- ** Color Picker for ESP Chams Starts Here **

do
    local initTbl = GetConfig("visuals.playerChamsColor", nil)
    local initColor = (type(initTbl) == "table" and initTbl.r and initTbl.g and initTbl.b) and Color3.new(initTbl.r, initTbl.g, initTbl.b) or COLORS.accent
    local api = ColorPickerAPI[playerChamsColorPicker]
    if api then
        api.OnChange = function(c)
            SetConfig("visuals.playerChamsColor", { r = c.R, g = c.G, b = c.B })
            pcall(function()
                for _, inst in ipairs(gui:GetChildren()) do
                    if inst:IsA("Highlight") then
                        if inst.Name == "Rivals_PlayerChams" then
                            inst.FillColor = c
                        elseif inst.Name == "Rivals_GlowChams" then
                            inst.FillColor = c
                            inst.OutlineColor = c
                        end
                    end
                end
                for _, p in ipairs(Players:GetPlayers()) do
                    local ch = p.Character
                    if ch then
                        for _, d in ipairs(ch:GetDescendants()) do
                            if d:IsA("PointLight") and d.Name == "Rivals_GlowLight" then
                                d.Color = c
                            end
                        end
                    end
                end
            end)
        end
        pcall(function() api.Set(initColor) end)
    end
end

--- ** Color Picker for ESP Chams Ends Here **

---------------------------------------------------------------------------

-- ** ESP Boxes Logic Starts Here **

do
    if typeof(Drawing) == "table" and Drawing.new then
        local boxes = {}
        local renderConn, playerAddedConn, playerRemovingConn
        local charConns = {}
        local colorApi = nil
        local colorApiPrev = nil

        local Players = game:GetService("Players")
        local RunService = game:GetService("RunService")
        local localPlayer = Players.LocalPlayer

        local MAX_CREATE_DISTANCE = 300 
        local PAD = 8

        local function getBoxColor()
            local okE, eApi = pcall(function() return ColorPickerAPI[espBoxesColorPicker] end)
            if okE and eApi and eApi.Get then
                local c = eApi.Get()
                if typeof(c) == "Color3" then return c end
            end
            local tbl = GetConfig("visuals.espBoxesColor", nil)
            if type(tbl) == "table" and tbl.r and tbl.g and tbl.b then
                return Color3.new(tbl.r, tbl.g, tbl.b)
            end
            local ok, api = pcall(function() return ColorPickerAPI[playerChamsColorPicker] end)
            if ok and api and api.Get then
                local c = api.Get()
                if typeof(c) == "Color3" then return c end
            end
            return COLORS.accent
        end

        local function makeBoxForPlayer(p)
            if boxes[p] then return boxes[p] end
            local ok, box = pcall(function() return Drawing.new("Square") end)
            if not ok or not box then return nil end
            box.Visible = false
            box.Filled = false
            box.Thickness = 2
            box.Color = getBoxColor()
            boxes[p] = box
            return box
        end

        local function removeBoxForPlayer(p)
            if boxes[p] then
                pcall(function() boxes[p]:Remove() end)
                boxes[p] = nil
            end
        end

        local function projectWorldPointsToScreen(cam, points)
            local minX, minY = math.huge, math.huge
            local maxX, maxY = -math.huge, -math.huge
            local anyOnScreen = false
            for _, worldPos in ipairs(points) do
                local ok, sx, sy, sz
                ok, sx, sy, sz = pcall(function() 
                    local xv = cam:WorldToViewportPoint(worldPos)
                    return xv.X, xv.Y, xv.Z
                end)
                if ok and sz and sz > 0 then
                    anyOnScreen = true
                    minX = math.min(minX, sx)
                    maxX = math.max(maxX, sx)
                    minY = math.min(minY, sy)
                    maxY = math.max(maxY, sy)
                end
            end
            return anyOnScreen and minX or nil, anyOnScreen and minY or nil, anyOnScreen and maxX or nil, anyOnScreen and maxY or nil
        end

        local function getImportantParts(ch)
            local parts = {}
            local function tryGet(name)
                local p = ch:FindFirstChild(name)
                if p and p:IsA("BasePart") then table.insert(parts, p) end
            end
            tryGet("HumanoidRootPart")
            tryGet("Head")
            tryGet("UpperTorso")
            tryGet("LowerTorso")
            return parts
        end

        local function updateBoxes()
            local cam = workspace.CurrentCamera
            if not cam then return end
            local color = getBoxColor()
            local camPos = cam.CFrame.Position

            for _, p in ipairs(Players:GetPlayers()) do
                if p == localPlayer then continue end
                local ch = p and p.Character
                if not ch or not ch.Parent then
                    removeBoxForPlayer(p)
                else
                    local root = ch.PrimaryPart or ch:FindFirstChild("HumanoidRootPart")
                    if not root then
                        removeBoxForPlayer(p)
                    else
                        local dist = (root.Position - camPos).Magnitude
                        if dist > MAX_CREATE_DISTANCE then
                            removeBoxForPlayer(p)
                        else
                            local box = boxes[p] or makeBoxForPlayer(p)
                            if not box then
                            else
                                local minX, minY, maxX, maxY
                                local ok, bboxCFrame, bboxSize = pcall(function() return ch:GetBoundingBox() end)
                                if ok and bboxCFrame and bboxSize then
                                    local hx, hy, hz = bboxSize.X / 2, bboxSize.Y / 2, bboxSize.Z / 2
                                    local corners = {
                                        bboxCFrame * CFrame.new(-hx, -hy, -hz),
                                        bboxCFrame * CFrame.new(-hx, -hy,  hz),
                                        bboxCFrame * CFrame.new(-hx,  hy, -hz),
                                        bboxCFrame * CFrame.new(-hx,  hy,  hz),
                                        bboxCFrame * CFrame.new( hx, -hy, -hz),
                                        bboxCFrame * CFrame.new( hx, -hy,  hz),
                                        bboxCFrame * CFrame.new( hx,  hy, -hz),
                                        bboxCFrame * CFrame.new( hx,  hy,  hz),
                                    }
                                    local points = {}
                                    for _, cf in ipairs(corners) do table.insert(points, cf.Position) end
                                    minX, minY, maxX, maxY = projectWorldPointsToScreen(cam, points)
                                else
                                    -- ** fallback to important parts
                                    local parts = getImportantParts(ch)
                                    local points = {}
                                    for _, part in ipairs(parts) do table.insert(points, part.Position) end
                                    minX, minY, maxX, maxY = projectWorldPointsToScreen(cam, points)
                                end

                                if not minX then
                                    box.Visible = false
                                else
                                    local x = minX - PAD
                                    local y = minY - PAD
                                    local w = math.max(4, maxX - minX + PAD * 2)
                                    local h = math.max(4, maxY - minY + PAD * 2)
                                    box.Position = Vector2.new(x, y)
                                    box.Size = Vector2.new(w, h)
                                    box.Color = color
                                    box.Visible = true
                                end
                            end
                        end
                    end
                end
            end
        end

        local function enableBoxes()
            for _, p in ipairs(Players:GetPlayers()) do
                if p ~= localPlayer then
                    if charConns[p] then pcall(function() charConns[p]:Disconnect() end) end
                    charConns[p] = p.CharacterAdded:Connect(function()
                        pcall(function() end) 
                    end)
                end
            end
            playerAddedConn = Players.PlayerAdded:Connect(function(p)
                if p ~= localPlayer then
                    if charConns[p] then pcall(function() charConns[p]:Disconnect() end) end
                    charConns[p] = p.CharacterAdded:Connect(function()
                        pcall(function() end)
                    end)
                end
            end)
            playerRemovingConn = Players.PlayerRemoving:Connect(function(p)
                if charConns[p] then pcall(function() charConns[p]:Disconnect() end) charConns[p] = nil end
                removeBoxForPlayer(p)
            end)
            pcall(function()
                colorApi = ColorPickerAPI[espBoxesColorPicker] or ColorPickerAPI[playerChamsColorPicker]
                if colorApi then
                    colorApiPrev = colorApi.OnChange
                    colorApi.OnChange = function(c)
                        if colorApiPrev then pcall(colorApiPrev, c) end
                        for _, b in pairs(boxes) do pcall(function() b.Color = c end) end
                    end
                end
            end)
            if not renderConn then renderConn = RunService.RenderStepped:Connect(updateBoxes) end
        end

        local function disableBoxes()
            if renderConn then pcall(function() renderConn:Disconnect() end) renderConn = nil end
            if playerAddedConn then pcall(function() playerAddedConn:Disconnect() end) playerAddedConn = nil end
            if playerRemovingConn then pcall(function() playerRemovingConn:Disconnect() end) playerRemovingConn = nil end
            for p,_ in pairs(charConns) do pcall(function() charConns[p]:Disconnect() end) charConns[p] = nil end
            for p,_ in pairs(boxes) do removeBoxForPlayer(p) end
            pcall(function()
                if colorApi and colorApi.OnChange then
                    colorApi.OnChange = colorApiPrev
                end
                colorApi = nil
                colorApiPrev = nil
            end)
        end

        local api = ToggleAPI[espBoxesToggle]
        if api then
            local prev = api.OnToggle
            api.OnToggle = function(state)
                if prev then pcall(prev, state) end
                if state then pcall(enableBoxes) else pcall(disableBoxes) end
            end
            pcall(function() if api.Get and api.Get() then enableBoxes() end end)
        end

        RegisterUnload(function()
            pcall(disableBoxes)
        end)
    end
end



-- ** ESP Boxes Logic Ends Here **

---------------------------------------------------------------------------

-- ** Glow Chams Logic Starts Here **
do
    local glow = {}
    local glowConns = {}
    local playerAddedConn, playerRemovingConn

    local function getSavedColor()
        local coltbl = GetConfig("visuals.playerChamsColor", nil)
        if type(coltbl) == "table" and coltbl.r and coltbl.g and coltbl.b then
            return Color3.new(coltbl.r, coltbl.g, coltbl.b)
        end
        return COLORS.accent
    end

    local function applyGlowToCharacter(char, intensity)
        if not char or not char:IsA("Model") then return nil end
        local ok, h = pcall(function()
            local inst = Instance.new("Highlight")
            inst.Name = "Rivals_GlowChams"
            inst.Adornee = char
            inst.FillColor = getSavedColor()
            inst.OutlineColor = getSavedColor()
            inst.DepthMode = Enum.HighlightDepthMode.AlwaysOnTop
            inst.Parent = gui
            local t = math.clamp(1 - (intensity or 50) / 100, 0, 1)
            inst.FillTransparency = t * 0.6 
            inst.OutlineTransparency = t * 0.35 
            local lights = {}
            local function makeLight(part, scale)
                if not part or not part:IsA("BasePart") then return nil end
                local pl = Instance.new("PointLight")
                pl.Name = "Rivals_GlowLight"
                pl.Color = inst.FillColor
                local rng = 6 + (intensity or 50) / 100 * (24 * (scale or 1)) 
                local bri = 1 + (intensity or 50) / 100 * (4 * (scale or 1))   
                pl.Range = rng
                pl.Brightness = bri
                pl.Shadows = false
                pl.Parent = part
                return pl
            end
            local head = char:FindFirstChild("Head")
            local hrp = char:FindFirstChild("HumanoidRootPart")
            if head and head:IsA("BasePart") then table.insert(lights, makeLight(head, 1.0)) end
            if hrp and hrp:IsA("BasePart") then table.insert(lights, makeLight(hrp, 0.7)) end
            if #lights == 0 then
                for _,part in ipairs(char:GetChildren()) do
                    if part:IsA("BasePart") then table.insert(lights, makeLight(part, 0.5)); break end
                end
            end
            return {hl = inst, lights = lights}
        end)
        if ok then return h end
        return nil
    end

    local function removeGlowFromPlayer(p)
        if glowConns[p] then
            pcall(function() glowConns[p]:Disconnect() end)
            glowConns[p] = nil
        end
        if glow[p] then
            pcall(function()
                local data = glow[p]
                if data.hl and data.hl.Destroy then data.hl:Destroy() end
                if data.lights and type(data.lights) == "table" then
                    for _,l in ipairs(data.lights) do
                        if l and l.Destroy then pcall(function() l:Destroy() end) end
                    end
                end
            end)
            glow[p] = nil
        end
    end

    local function addGlowToPlayer(p, intensity)
        if not p or p == player then return end
        removeGlowFromPlayer(p)
        if p.Character then
            glow[p] = applyGlowToCharacter(p.Character, intensity)
        end
        glowConns[p] = p.CharacterAdded:Connect(function(c)
            pcall(function() removeGlowFromPlayer(p) end)
            pcall(function() glow[p] = applyGlowToCharacter(c, intensity) end)
        end)
    end

    local function enableGlow()
        disableGlow = disableGlow 
        local intensity = GetConfig("visuals.glowChamsIntensity", 50)
        playerAddedConn = Players.PlayerAdded:Connect(function(p) addGlowToPlayer(p, intensity) end)
        playerRemovingConn = Players.PlayerRemoving:Connect(function(p) removeGlowFromPlayer(p) end)
        for _,p in ipairs(Players:GetPlayers()) do
            addGlowToPlayer(p, intensity)
        end
    end

    local function disableGlow()
        if playerAddedConn then pcall(function() playerAddedConn:Disconnect() end) playerAddedConn = nil end
        if playerRemovingConn then pcall(function() playerRemovingConn:Disconnect() end) playerRemovingConn = nil end
        for p,_ in pairs(glowConns) do
            pcall(function() glowConns[p]:Disconnect() end)
            glowConns[p] = nil
        end
        for p,data in pairs(glow) do
            pcall(function()
                if data and type(data) == "table" then
                    if data.hl and data.hl.Destroy then pcall(function() data.hl:Destroy() end) end
                    if data.lights and type(data.lights) == "table" then
                        for _,l in ipairs(data.lights) do if l and l.Destroy then pcall(function() l:Destroy() end) end end
                    end
                end
            end)
            glow[p] = nil
        end
    end

    local initialIntensity = GetConfig("visuals.glowChamsIntensity", 50)
    local sliderApi = SliderAPI[glowIntensitySlider]
    if sliderApi then
        sliderApi.OnChange = function(v)
            SetConfig("visuals.glowChamsIntensity", v)
            pcall(function()
                for _,data in pairs(glow) do
                            if data and type(data) == "table" then
                                local t = math.clamp(1 - v / 100, 0, 1)
                                if data.hl and data.hl.IsA and data.hl:IsA("Highlight") then
                                    data.hl.FillTransparency = t * 0.6
                                    data.hl.OutlineTransparency = t * 0.35
                                end
                                if data.lights and type(data.lights) == "table" then
                                    for _,l in ipairs(data.lights) do
                                        if l and l.IsA and l:IsA("PointLight") then
                                            l.Range = 6 + v / 100 * 24
                                            l.Brightness = 1 + v / 100 * 4
                                        end
                                    end
                                end
                            end
                        end
            end)
        end
        pcall(function() sliderApi.Set(initialIntensity) end)
    end

    BindToggleToConfig(glowChamsToggle, "visuals.glowChams", false)
    do
        local api = ToggleAPI[glowChamsToggle]
        if api then
            local prev = api.OnToggle
            api.OnToggle = function(state)
                if prev then pcall(prev, state) end
                if state then pcall(enableGlow) else pcall(disableGlow) end
            end
            pcall(function() if api.Get and api.Get() then enableGlow() end end)
        end
    end

    RegisterUnload(function()
        pcall(disableGlow)
    end)
end
-- ** Glow Chams Logic Ends Here **

---------------------------------------------------------------------------

-- ** Close/Open GUI on Keybind logic Starts Here **

            do
                local KEY_CONFIG = "settings.closeOpenGuiKey"
                local keyApi = KeybindAPI[closeOpenGuiKeybind]

                pcall(function()
                    local saved = GetConfig(KEY_CONFIG, "Insert")
                    if keyApi and type(saved) == "string" and Enum.KeyCode[saved] then
                        pcall(function() keyApi.Set(Enum.KeyCode[saved]) end)
                    end
                end)

                if keyApi then
                    keyApi.OnBind = function(k)
                        local name = nil
                        if typeof(k) == "EnumItem" then
                            name = k.Name
                        elseif type(k) == "string" then
                            name = tostring(k)
                        end
                        if name then SetConfig(KEY_CONFIG, name) end
                    end
                end

                local keyConn
                keyConn = UserInputService.InputBegan:Connect(function(input, gameProcessed)
                    if gameProcessed then return end
                    if input.UserInputType ~= Enum.UserInputType.Keyboard then return end
                    local bound = GetConfig(KEY_CONFIG, "Insert")
                    if not bound then return end
                    local target = Enum.KeyCode[bound]
                    if not target then return end
                    if input.KeyCode == target then
                        pcall(function()
                            if root and root.Parent then
                                root.Visible = not root.Visible
                            end
                        end)
                    end
                end)

                RegisterUnload(function()
                    if keyConn and keyConn.Disconnect then
                        pcall(function() keyConn:Disconnect() end)
                        keyConn = nil
                    end
                end)
            end



-- ** Close/Open GUI on Keybind logic Ends Here **

---------------------------------------------------------------------------


-- ** Show GUI on Load logic Starts Here **
do
    local KEY = "settings.showGuiOnLoad"
    local api = ToggleAPI[showGuiOnLoadToggle]

    -- ** apply save config on load
    pcall(function()
        local show = GetConfig(KEY, true)
        if root and root.Parent then root.Visible = not not show end
    end)

    -- ** bind toggle to config and runtime
    if api then
        local prev = api.OnToggle
        api.OnToggle = function(state)
            if prev then pcall(prev, state) end
            pcall(function() if root and root.Parent then root.Visible = not not state end end)
        end
        pcall(function() if api.Get and api.Get() then root.Visible = true else root.Visible = false end end)
    end
end

-- ** Show GUI on Load logic Ends Here **

---------------------------------------------------------------------------

-- ** Player Health Logic Starts Here **

do
    local Players = game:GetService("Players")
    local RunService = game:GetService("RunService")
    local UserInputService = game:GetService("UserInputService")

    local healthOverlays = {}
    local charConns = {}
    local humConns = {}

    local HEALTH_BASE_W, HEALTH_BASE_H = 140, 12  
    local MAX_CREATE_DISTANCE = 350
    local HEALTH_BAR_MIN_WIDTH = 60
    local HEALTH_BAR_MAX_WIDTH = 140

    local KEY_CONFIG = "settings.showHealthKey"

    local function safeDisconnect(c)
        if c and c.Disconnect then
            pcall(function() c:Disconnect() end)
        end
    end

    local function createHealthBar(p)
        if healthOverlays[p] then return healthOverlays[p] end
        
        local bg = Drawing.new("Square")
        local fill = Drawing.new("Square")
        local text = Drawing.new("Text")
        
        bg.Filled = true
        bg.Thickness = 1
        bg.Color = Color3.fromRGB(30, 30, 30)
        bg.ZIndex = 1
        bg.Visible = false
        
        fill.Filled = true
        fill.Thickness = 0
        fill.Color = Color3.fromRGB(0, 200, 80)
        fill.ZIndex = 2
        fill.Visible = false
        
        text.Center = true
        text.Outline = true
        text.Font = 2
        text.Size = 14  
        text.Color = Color3.new(1, 1, 1)
        text.ZIndex = 3
        text.Visible = false
        
        healthOverlays[p] = {
            bg = bg,
            fill = fill,
            text = text
        }
        
        return healthOverlays[p]
    end

    local function removeHealthBar(p)
        local data = healthOverlays[p]
        if not data then return end
        
        if data.bg then data.bg:Remove() end
        if data.fill then data.fill:Remove() end
        if data.text then data.text:Remove() end
        
        healthOverlays[p] = nil
    end

    local function updateHealthBar(p, data, cam, refPos)
        if not p.Character then 
            data.bg.Visible = false
            data.fill.Visible = false
            data.text.Visible = false
            return 
        end
        
        local root = p.Character.PrimaryPart or p.Character:FindFirstChild("HumanoidRootPart")
        if not root then 
            data.bg.Visible = false
            data.fill.Visible = false
            data.text.Visible = false
            return 
        end
        
        local dist = (root.Position - refPos).Magnitude
        if dist > MAX_CREATE_DISTANCE then
            data.bg.Visible = false
            data.fill.Visible = false
            data.text.Visible = false
            return
        end
        
        local ok, bboxCFrame, bboxSize = pcall(function() 
            return p.Character:GetBoundingBox() 
        end)
        
        if not ok then 
            data.bg.Visible = false
            data.fill.Visible = false
            data.text.Visible = false
            return 
        end
        
        local minX, minY = math.huge, math.huge
        local maxX, maxY = -math.huge, -math.huge
        
        local hx, hy, hz = bboxSize.X / 2, bboxSize.Y / 2, bboxSize.Z / 2
        local corners = {
            bboxCFrame * CFrame.new(-hx, -hy, -hz),
            bboxCFrame * CFrame.new(-hx, -hy,  hz),
            bboxCFrame * CFrame.new(-hx,  hy, -hz),
            bboxCFrame * CFrame.new(-hx,  hy,  hz),
            bboxCFrame * CFrame.new( hx, -hy, -hz),
            bboxCFrame * CFrame.new( hx, -hy,  hz),
            bboxCFrame * CFrame.new( hx,  hy, -hz),
            bboxCFrame * CFrame.new( hx,  hy,  hz),
        }
        
        local anyVisible = false
        for _, cf in ipairs(corners) do
            local screen = cam:WorldToViewportPoint(cf.Position)
            if screen.Z > 0 then
                anyVisible = true
                minX = math.min(minX, screen.X)
                maxX = math.max(maxX, screen.X)
                minY = math.min(minY, screen.Y)
                maxY = math.max(maxY, screen.Y)
            end
        end
        
        if not anyVisible then
            data.bg.Visible = false
            data.fill.Visible = false
            data.text.Visible = false
            return
        end
        
        local espWidth = maxX - minX
        
        local scaleFactor = math.clamp(1 - (dist / MAX_CREATE_DISTANCE) * 0.5, 0.3, 1.0)
        local healthBarWidth = math.clamp(espWidth * 0.8 * scaleFactor, HEALTH_BAR_MIN_WIDTH, HEALTH_BAR_MAX_WIDTH)
        local healthBarHeight = 10  
        
        local centerX = (minX + maxX) / 2
        local yPos = minY - healthBarHeight - 8
        
        local hum = p.Character:FindFirstChildOfClass("Humanoid")
        local pct = hum and math.clamp(hum.Health / math.max(hum.MaxHealth, 1), 0, 1) or 0
        
        data.bg.Size = Vector2.new(healthBarWidth, healthBarHeight)
        data.bg.Position = Vector2.new(centerX - healthBarWidth/2, yPos)
        data.bg.Visible = true
        
        local fillWidth = math.max(2, healthBarWidth * pct)
        data.fill.Size = Vector2.new(fillWidth, healthBarHeight)
        data.fill.Position = Vector2.new(centerX - healthBarWidth/2, yPos)
        data.fill.Visible = true
        
        local hp = math.floor((hum and hum.Health) or 0)
        local max = math.floor((hum and hum.MaxHealth) or 0)
        data.text.Text = string.format("%d/%d", hp, max)
        data.text.Position = Vector2.new(centerX, yPos + healthBarHeight/2 - 1)

        local textSize = math.clamp(math.floor(healthBarWidth / 6), 12, 16)  
        data.text.Size = textSize
        data.text.Visible = true
    end

    local renderConn
    local function onRender()
        local cam = workspace.CurrentCamera
        if not cam then return end
        
        local refPos = cam.CFrame.Position
        
        for _, player in ipairs(Players:GetPlayers()) do
            if player == Players.LocalPlayer then continue end
            
            local data = healthOverlays[player]
            
            if not data then
                data = createHealthBar(player)
            end
            
            if data then
                updateHealthBar(player, data, cam, refPos)
            end
        end
    end

    local function addPlayer(p)
        if p == Players.LocalPlayer then return end
        
        charConns[p] = p.CharacterAdded:Connect(function()
            createHealthBar(p)
        end)
        
        humConns[p] = nil
        
        if p.Character then
            local hum = p.Character:FindFirstChildOfClass("Humanoid")
            if hum then
                humConns[p] = hum.HealthChanged:Connect(function()
                end)
            end
        end
        
        local charAddedConn
        charAddedConn = p.CharacterAdded:Connect(function(char)
            task.wait(0.5)
            local hum = char:FindFirstChildOfClass("Humanoid")
            if hum then
                humConns[p] = hum.HealthChanged:Connect(function()
                end)
            end
        end)
        
        if charConns[p] ~= charAddedConn then
            table.insert(charConns, charAddedConn)
        end
    end

    local function removePlayer(p)
        safeDisconnect(charConns[p])
        charConns[p] = nil
        
        safeDisconnect(humConns[p])
        humConns[p] = nil
        
        removeHealthBar(p)
    end

    local playerAddedConn, playerRemovingConn
    local function enableHealth()
        for _, p in ipairs(Players:GetPlayers()) do
            addPlayer(p)
        end
        
        playerAddedConn = Players.PlayerAdded:Connect(addPlayer)
        playerRemovingConn = Players.PlayerRemoving:Connect(removePlayer)
        
        if not renderConn then
            renderConn = RunService.RenderStepped:Connect(onRender)
        end
    end

    local function disableHealth()
        if renderConn then
            renderConn:Disconnect()
            renderConn = nil
        end
        
        if playerAddedConn then
            playerAddedConn:Disconnect()
            playerAddedConn = nil
        end
        
        if playerRemovingConn then
            playerRemovingConn:Disconnect()
            playerRemovingConn = nil
        end
        
        for p, _ in pairs(charConns) do
            safeDisconnect(charConns[p])
        end
        charConns = {}
        
        for p, _ in pairs(humConns) do
            safeDisconnect(humConns[p])
        end
        humConns = {}
        
        for p, _ in pairs(healthOverlays) do
            removeHealthBar(p)
        end
        healthOverlays = {}
    end

    local api = ToggleAPI[playerHealthToggle]
    if api then
        local prev = api.OnToggle
        api.OnToggle = function(state)
            if prev then pcall(prev, state) end
            if state then enableHealth() else disableHealth() end
        end
        if api.Get and api.Get() then
            enableHealth()
        end
    end

    local keyApi = KeybindAPI[showHealthKeybind]
    if keyApi then
        local saved = GetConfig(KEY_CONFIG, "P")
        if type(saved) == "string" and Enum.KeyCode[saved] then
            keyApi.Set(Enum.KeyCode[saved])
        end
        
        keyApi.OnBind = function(k)
            local name = k.Name or tostring(k)
            SetConfig(KEY_CONFIG, name)
        end
    end

    local keyConn = UserInputService.InputBegan:Connect(function(input, gameProcessed)
        if gameProcessed or input.UserInputType ~= Enum.UserInputType.Keyboard then return end
        
        local bound = GetConfig(KEY_CONFIG, "P")
        local target = bound and Enum.KeyCode[bound]
        if target and input.KeyCode == target then
            local tapi = ToggleAPI[playerHealthToggle]
            if tapi and tapi.Get and tapi.Set then
                tapi.Set(not tapi.Get())
            end
        end
    end)

    RegisterUnload(function()
        if keyConn then keyConn:Disconnect() end
        disableHealth()
    end)
end

-- ** Player Health Logic Ends Here **

---------------------------------------------------------------------------

-- ** Aimbot Logic Starts Here **
do
    local KEY_CONFIG = "combat.enableAimbotKey"
    local fovMax = GetConfig("combat.aimbotFOV", 700) or 700
    local leftDown, rightDown = false, false
    local loopConn, inputConnBegin, inputConnEnd, fovDrawConn

    local smoothingEnabled = false
    local smoothingValue = GetConfig("combat.aimbotSmoothing", 1) or 1
    local aimAccumX, aimAccumY = 0, 0
    local teamCheckEnabled = GetConfig("combat.teamCheck", true) or true
    local teammateCache = {}
    local aimHistory = {} 
    local projSpeedLocal = (type(projSpeed) == "number" and projSpeed) or 900
    local leadScaleLocal = (type(leadScale) == "number" and leadScale) or 1
    local aimPredictionEnabled = GetConfig("combat.aimPrediction", false) or false

    local t = ToggleAPI[aimbotToggle]
    if t then
        local p = t.OnToggle
        t.OnToggle = function(s)
            if type(p) == "function" then p(s) end
            makeNotification(s and "Aimbot is ON" or "Aimbot is OFF", 3, nil, "AimbotToggle")
        end
    end

    -- ** Team check api ** --
    _G.RivalsCHT_TeamCheck = _G.RivalsCHT_TeamCheck or {}
    do
        local teamApi = _G.RivalsCHT_TeamCheck
        teamApi.GetCache = function() return teammateCache end
        teamApi.IsTeammate = function(playerOrName)
            if not playerOrName then return false end
            local Players = game:GetService("Players")
            local pl = nil
            if type(playerOrName) == "string" then
                pl = Players:FindFirstChild(playerOrName)
                if not pl then return false end
            else
                pl = playerOrName
            end
            local entry = teammateCache[pl]
            if entry and entry.isTeam ~= nil then return entry.isTeam end
            local ok, isTeam = pcall(function()
                local ch = pl.Character
                local head = ch and (ch:FindFirstChild("Head") or ch:FindFirstChild("HumanoidRootPart"))
                if head then
                    local hrp = head
                    if hrp and hrp.Name ~= "HumanoidRootPart" then
                        hrp = head.Parent and head.Parent:FindFirstChild("HumanoidRootPart")
                    end
                    if hrp then
                        local function findLabelNow()
                            local ok, found = pcall(function()
                                local f = hrp:FindFirstChild("TeammateLabel", true)
                                if f then return f end
                                if ch then
                                    f = ch:FindFirstChild("TeammateLabel", true)
                                    if f then return f end
                                end
                                local wp = workspace:FindFirstChild(pl.Name)
                                if wp then
                                    f = wp:FindFirstChild("TeammateLabel", true)
                                    if f then return f end
                                end
                                return nil
                            end)
                            if ok and found then return found end
                            return nil
                        end

                        local lbl = findLabelNow()
                        if not lbl then
                            pcall(function()
                                if task and task.delay then
                                    task.delay(1, function()
                                        local late = findLabelNow()
                                        if late then
                                            teammateCache[pl] = { hrp = hrp, isTeam = true }
                                        end
                                    end)
                                else
                                    spawn(function() wait(1) local late = findLabelNow() if late then teammateCache[pl] = { hrp = hrp, isTeam = true } end end)
                                end
                            end)
                            return false
                        end

                        teammateCache[pl] = { hrp = hrp, isTeam = true }
                        return true
                    end
                end
                return false
            end)
            return ok and isTeam or false
        end
        teamApi.GetTeammates = function()
            local t = {}
            for p,v in pairs(teammateCache) do if v and v.isTeam then table.insert(t,p) end end
            return t
        end
        teamApi.Invalidate = function(playerOrName)
            local Players = game:GetService("Players")
            if not playerOrName then for k in pairs(teammateCache) do teammateCache[k] = nil end; return end
            local pl = (type(playerOrName) == "string") and Players:FindFirstChild(playerOrName) or playerOrName
            if pl then teammateCache[pl] = nil end
        end
    end

    -- ** end of team check ** --
    local playerRemoveConn
    pcall(function()
        playerRemoveConn = Players.PlayerRemoving:Connect(function(p)
            teammateCache[p] = nil
        end)
    end)

    local playerCharConns = {}
    pcall(function()
        for _, pl in ipairs(Players:GetPlayers()) do
            if pl ~= Players.LocalPlayer then
                if playerCharConns[pl] then pcall(function() playerCharConns[pl]:Disconnect() end) end
                playerCharConns[pl] = pl.CharacterAdded:Connect(function()
                    teammateCache[pl] = nil
                end)
            end
        end
        Players.PlayerAdded:Connect(function(pl)
            if pl ~= Players.LocalPlayer then
                playerCharConns[pl] = pl.CharacterAdded:Connect(function()
                    teammateCache[pl] = nil
                end)
            end
        end)
    end)

    if GetConfig("combat.aimbotSmoothing", nil) == nil then SetConfig("combat.aimbotSmoothing", smoothingValue) end
    if GetConfig("combat.aimbotFOV", nil) == nil then SetConfig("combat.aimbotFOV", fovMax) end
    if GetConfig("combat.useAimbotSmoothing", nil) == nil then SetConfig("combat.useAimbotSmoothing", smoothingEnabled) end

    local function safeDisconnect(c)
        if c and c.Disconnect then pcall(function() c:Disconnect() end) end
    end

    local keyApi = KeybindAPI[enableAimbotKeybind]
    pcall(function()
        local saved = GetConfig(KEY_CONFIG, "V")
        if keyApi and type(saved) == "string" and Enum.KeyCode[saved] then pcall(function() keyApi.Set(Enum.KeyCode[saved]) end) end
    end)
    if keyApi then
        keyApi.OnBind = function(k)
            local name = nil
            if typeof(k) == "EnumItem" then name = k.Name elseif type(k) == "string" then name = tostring(k) end
            if name then SetConfig(KEY_CONFIG, name) end
        end
    end

    do
        local tApi = ToggleAPI[useAimbotSmoothingToggle]
        local sApi = SliderAPI[smoothingSlider]
        if tApi then
            smoothingEnabled = tApi.Get and tApi.Get() or false
            local prev = tApi.OnToggle
            tApi.OnToggle = function(state)
                if prev then pcall(prev, state) end
                smoothingEnabled = not not state
                SetConfig("combat.useAimbotSmoothing", smoothingEnabled)
                pcall(function() if debugDelayLabel then debugDelayLabel.Show(debugModeEnabled and not smoothingEnabled) end end)
            end
        end
        if sApi then
            smoothingValue = sApi.Get and sApi.Get() or smoothingValue
            sApi.OnChange = function(v)
                smoothingValue = tonumber(v) or smoothingValue
                SetConfig("combat.aimbotSmoothing", smoothingValue)
            end
            pcall(function() sApi.Set(smoothingValue) end)
            pcall(function() if debugDelayLabel then debugDelayLabel.Show(debugModeEnabled and not smoothingEnabled) end end)
        end
    end

    do
        local tApi = ToggleAPI[teamCheckToggle]
        if tApi then
            teamCheckEnabled = tApi.Get and tApi.Get() or teamCheckEnabled
            local prev = tApi.OnToggle
            tApi.OnToggle = function(state)
                if prev then pcall(prev, state) end
                teamCheckEnabled = not not state
                SetConfig("combat.teamCheck", teamCheckEnabled)
                pcall(function()
                    for k in pairs(teammateCache) do teammateCache[k] = nil end
                end)
            end
        end
    end

    do
        local tApi = ToggleAPI[aimPredictionToggle]
        if tApi then
            aimPredictionEnabled = tApi.Get and tApi.Get() or aimPredictionEnabled
            local prev = tApi.OnToggle
            tApi.OnToggle = function(state)
                if prev then pcall(prev, state) end
                aimPredictionEnabled = not not state
                SetConfig("combat.aimPrediction", aimPredictionEnabled)
            end
        end
    end

    do
        local fApi = SliderAPI[aimbotFOVSlider]
        if fApi then
            fovMax = fApi.Get and fApi.Get() or fovMax
            fApi.OnChange = function(v)
                fovMax = tonumber(v) or fovMax
                SetConfig("combat.aimbotFOV", fovMax)
            end
            pcall(function() fApi.Set(fovMax) end)
        end
    end

    local drawCircle = nil
    local drawEnabled = false
    do
        local dApi = ToggleAPI[drawFovCircleToggle]
        if dApi then
            drawEnabled = dApi.Get and dApi.Get() or false
            local prev = dApi.OnToggle
            dApi.OnToggle = function(s)
                if prev then pcall(prev, s) end
                drawEnabled = not not s
                pcall(function()
                    if drawEnabled then
                        if not drawCircle and typeof(Drawing) == "table" and Drawing.new then
                            drawCircle = Drawing.new("Circle")
                            drawCircle.Color = Color3.fromRGB(255,255,255)
                            drawCircle.Thickness = 1
                            drawCircle.Filled = false
                            drawCircle.Visible = true
                        end
                        if not fovDrawConn then
                            fovDrawConn = RunService.RenderStepped:Connect(function()
                                if not drawCircle then return end
                                if not drawEnabled then
                                    pcall(function() drawCircle.Visible = false end)
                                    return
                                end
                                local cam = workspace.CurrentCamera
                                if not cam then pcall(function() drawCircle.Visible = false end); return end
                                local vs = cam.ViewportSize
                                pcall(function()
                                    drawCircle.Position = Vector2.new(vs.X * 0.5, vs.Y * 0.5)
                                    drawCircle.Radius = fovMax
                                    drawCircle.Visible = true
                                end)
                            end)
                        end
                    else
                        if fovDrawConn then pcall(function() fovDrawConn:Disconnect() end) fovDrawConn = nil end
                        if drawCircle and drawCircle.Remove then pcall(function() drawCircle:Remove() end) end
                        drawCircle = nil
                    end
                end)
            end
        end
        pcall(function()
            if drawEnabled and not drawCircle and typeof(Drawing) == "table" and Drawing.new then
                drawCircle = Drawing.new("Circle")
                drawCircle.Color = Color3.fromRGB(255,255,255)
                drawCircle.Thickness = 1
                drawCircle.Filled = false
                drawCircle.Visible = true
                if not fovDrawConn then
                    fovDrawConn = RunService.RenderStepped:Connect(function()
                        if not drawCircle then return end
                        if not drawEnabled then pcall(function() drawCircle.Visible = false end); return end
                        local cam = workspace.CurrentCamera
                        if not cam then pcall(function() drawCircle.Visible = false end); return end
                        local vs = cam.ViewportSize
                        pcall(function()
                            drawCircle.Position = Vector2.new(vs.X * 0.5, vs.Y * 0.5)
                            drawCircle.Radius = fovMax
                            drawCircle.Visible = true
                        end)
                    end)
                end
            end
        end)
    end

    local function findClosestHead()
        local cam = workspace.CurrentCamera
        if not cam then return nil end
        local vs = cam.ViewportSize
        local cx, cy = vs.X * 0.5, vs.Y * 0.5
        local best, bestDist = nil, math.huge
        for _,pl in ipairs(Players:GetPlayers()) do
            if pl ~= Players.LocalPlayer then
                local ch = pl.Character
                if ch then
                        local okAlive, aliveRes = pcall(function() return _G.RivalsCHT_Aimbot.IsAlive(ch) end)
                        if not okAlive or not aliveRes then continue end
                        local head = ch:FindFirstChild("Head") or ch:FindFirstChild("HumanoidRootPart")
                        if head and head.Position then
                        local isTeammate = false
                        if teamCheckEnabled then
                            local hrp = head
                            if hrp and hrp.Name ~= "HumanoidRootPart" then
                                hrp = head.Parent and head.Parent:FindFirstChild("HumanoidRootPart")
                            end
                            if hrp then
                                local cache = teammateCache[pl]
                                if cache and cache.hrp == hrp then
                                    isTeammate = cache.isTeam
                                else
                                    local function findLabelNow()
                                        local ok, found = pcall(function()
                                            local f = hrp:FindFirstChild("TeammateLabel", true)
                                            if f then return f end
                                            if ch then
                                                f = ch:FindFirstChild("TeammateLabel", true)
                                                if f then return f end
                                            end
                                            local wp = workspace:FindFirstChild(pl.Name)
                                            if wp then
                                                f = wp:FindFirstChild("TeammateLabel", true)
                                                if f then return f end
                                            end
                                            return nil
                                        end)
                                        if ok and found then return found end
                                        return nil
                                    end

                                    local lbl = findLabelNow()
                                    if not lbl then
                                        pcall(function()
                                            if task and task.delay then
                                                task.delay(1, function()
                                                    local late = findLabelNow()
                                                    if late then teammateCache[pl] = { hrp = hrp, isTeam = true } end
                                                end)
                                            else
                                                spawn(function() wait(1) local late = findLabelNow() if late then teammateCache[pl] = { hrp = hrp, isTeam = true } end end)
                                            end
                                        end)
                                        isTeammate = false
                                    else
                                        isTeammate = true
                                        teammateCache[pl] = { hrp = hrp, isTeam = true }
                                    end
                                end
                            end
                        end
                        if not isTeammate then
                            local p = cam:WorldToViewportPoint(head.Position)
                            if p.Z > 0 then
                            local occluded = false
                            if not targetBehindWallsEnabled then
                                pcall(function()
                                    local rp = RaycastParams.new()
                                    rp.FilterType = Enum.RaycastFilterType.Blacklist
                                    rp.FilterDescendantsInstances = { ch }
                                    local origin = cam.CFrame.Position
                                    local direction = head.Position - origin
                                    local ray = workspace:Raycast(origin, direction, rp)
                                    if ray and ray.Instance and not ray.Instance:IsDescendantOf(ch) then
                                        occluded = true
                                    end
                                end)
                            end
                            if not occluded then
                                local dx = p.X - cx
                                local dy = p.Y - cy
                                local dist = math.sqrt(dx*dx + dy*dy)
                                if dist < bestDist and dist <= fovMax then
                                    bestDist = dist
                                    best = head
                                end
                            end
                            end
                        end
                    end
                end
            end
        end
        return best, bestDist
    end

    local debugModeEnabled = GetConfig("settings.debugMode", false) or false
    local debugTrackerLabel, debugDelayLabel = nil, nil
    do
        debugTrackerLabel = makeDebugLabel("")
        debugDelayLabel = makeDebugLabel("")
        local tApi = ToggleAPI[debugModeToggle]
        if tApi then
            debugModeEnabled = tApi.Get and tApi.Get() or debugModeEnabled
            local prev = tApi.OnToggle
            tApi.OnToggle = function(s)
                if prev then pcall(prev, s) end
                debugModeEnabled = not not s
                if debugTrackerLabel then debugTrackerLabel.Show(debugModeEnabled) end
                if debugDelayLabel then debugDelayLabel.Show(debugModeEnabled and not smoothingEnabled) end
            end
        end
        if debugTrackerLabel then debugTrackerLabel.Show(debugModeEnabled) end
        if debugDelayLabel then debugDelayLabel.Show(debugModeEnabled and not smoothingEnabled) end
        RegisterUnload(function()
            if debugTrackerLabel then debugTrackerLabel.Destroy() end
            if debugDelayLabel then debugDelayLabel.Destroy() end
        end)
    end

    local persistentTarget = nil


    local function startLoop()
        if loopConn then return end
        loopConn = RunService.RenderStepped:Connect(function()
            local forceActive = (_G.RivalsCHT_Aimbot and _G.RivalsCHT_Aimbot.ForceActive) or false
            if not leftDown and not forceActive then return end
            local api = ToggleAPI[aimbotToggle]
            local enabled = api and api.Get and api.Get()
            if not enabled and not forceActive then return end
            local cam = workspace.CurrentCamera
            if not cam then return end

            local head = findClosestHead()
            local pApi = ToggleAPI[persistentAimbotToggle]
            local persistentEnabled = pApi and pApi.Get and pApi.Get()
            local now = tick()
            if head and head.Position then
                if persistentEnabled then
                    persistentTarget = { model = head.Parent, player = Players:GetPlayerFromCharacter(head.Parent), lastPos = head.Position, t = now }
                    if type(_G) == "table" and _G.RivalsCHT_Aimbot then _G.RivalsCHT_Aimbot.PersistentTarget = persistentTarget end
                end
            else
                if persistentEnabled and persistentTarget and persistentTarget.model and persistentTarget.model.Parent then
                    local model = persistentTarget.model
                    local reacquire = model:FindFirstChild("Head") or model:FindFirstChild("UpperTorso") or model:FindFirstChild("HumanoidRootPart")
                    if reacquire and reacquire.Position then
                        head = reacquire
                        persistentTarget.lastPos = reacquire.Position
                        persistentTarget.t = now
                    else
                        local timeout = 3 
                        if persistentTarget.lastPos and (now - (persistentTarget.t or 0) <= timeout) then
                            head = { Position = persistentTarget.lastPos }
                        else
                            persistentTarget = nil
                            if type(_G) == "table" and _G.RivalsCHT_Aimbot then _G.RivalsCHT_Aimbot.PersistentTarget = nil end
                        end
                    end
                end
            end
            if head and head.Position then
                pcall(function()
                    local predicted = head.Position
                    local root = head.Parent and (head.Parent:FindFirstChild("HumanoidRootPart") or head.Parent:FindFirstChild("Torso"))
                    local now = tick()
                    local estVel = nil
                    if root and root:IsA("BasePart") then
                        estVel = root.Velocity
                    end
                    local histId = (head.Parent and head.Parent.Name) or tostring(head)
                    local prev = aimHistory[histId]
                    pcall(function()
                        if debugModeEnabled and debugTrackerLabel then
                            local method = smoothingEnabled and "mousemoverel (smoothed)" or "mousemoverel (unsmoothed)"
                            debugTrackerLabel.Set("Using " .. method .. " ; smoothing is " .. (smoothingEnabled and "ON" or "OFF"))
                        end
                        if debugModeEnabled and not smoothingEnabled and debugDelayLabel then
                            local delayMs = 0
                            if prev and prev.t then delayMs = (now - prev.t) * 1000 end
                            debugDelayLabel.Set("Delay between head and cursor: " .. tostring(math.floor(delayMs)) .. " ms")
                        end
                    end)
                    if (not estVel or (estVel and estVel.Magnitude < 0.001)) and prev and prev.pos and prev.t then
                        local dt = now - prev.t
                        if dt > 0 then
                            estVel = (head.Position - prev.pos) / dt
                        end
                    end
                    aimHistory[histId] = { pos = head.Position, t = now }

                    if root and root:IsA("BasePart") then
                        local okDist, dist = pcall(function() return (head.Position - cam.CFrame.Position).Magnitude end)
                        local travel = (type(projSpeedLocal) == "number" and projSpeedLocal) or 900
                        if okDist and dist and travel and travel > 0 then
                            local tt = dist / travel
                                if estVel and aimPredictionEnabled then
                                    local dir = (head.Position - cam.CFrame.Position)
                                    local dirUnit = (dir.Magnitude > 0) and (dir / dir.Magnitude) or Vector3.new(0,0,0)
                                    local forwardComp = estVel:Dot(dirUnit)
                                    local lateral = estVel - dirUnit * forwardComp
                                    local lateralMag = lateral.Magnitude
                                    local leadFactor = 1
                                    if tt < 0.04 then
                                        leadFactor = 0
                                    elseif tt < 0.12 then
                                        leadFactor = (tt - 0.04) / (0.12 - 0.04)
                                    else
                                        leadFactor = 1
                                    end
                                    predicted = predicted + lateral * tt * leadScaleLocal * leadFactor
                                end
                        end
                    end

                    local p = cam:WorldToViewportPoint(predicted)
                    if (leftDown or forceActive) and persistentEnabled and p and p.Z and p.Z <= 0 then
                        pcall(function()
                            local camPos = cam.CFrame.Position
                            local dir = (predicted - camPos)
                            local look = cam.CFrame.LookVector
                            local desiredYaw = math.atan2(dir.X, dir.Z)
                            local currentYaw = math.atan2(look.X, look.Z)
                            local deltaYaw = desiredYaw - currentYaw
                            if deltaYaw > math.pi then deltaYaw = deltaYaw - 2 * math.pi end
                            if deltaYaw < -math.pi then deltaYaw = deltaYaw + 2 * math.pi end
                            local desiredPitch = math.atan2(dir.Y, math.sqrt(dir.X * dir.X + dir.Z * dir.Z))
                            local currentPitch = math.atan2(look.Y, math.sqrt(look.X * look.X + look.Z * look.Z))
                            local deltaPitch = desiredPitch - currentPitch
                            local vFov = math.rad(cam.FieldOfView or 70)
                            local vs = cam.ViewportSize
                            local aspect = (vs.X > 0 and vs.Y > 0) and (vs.X / vs.Y) or 1
                            local hFov = 2 * math.atan(math.tan(vFov * 0.5) * aspect)
                            local pxPerRadX = (hFov ~= 0) and (vs.X / hFov) or vs.X
                            local pxPerRadY = (vFov ~= 0) and (vs.Y / vFov) or vs.Y
                            local moveX = deltaYaw * pxPerRadX
                            local moveY = -deltaPitch * pxPerRadY
                            mousemoverel(moveX, moveY)
                        end)
                    end
                    if (leftDown or forceActive) and p.Z and p.Z > 0 then
                        local mousePos = UserInputService:GetMouseLocation()
                        local dx = p.X - mousePos.X
                        local dy = p.Y - mousePos.Y
                        local dist = math.sqrt(dx*dx + dy*dy)
                        if dist > 0.5 then
                            if smoothingEnabled then
                                local sv = tonumber(smoothingValue) or 1
                                if sv <= 0 then sv = 1 end
                                aimAccumX = aimAccumX + (dx / sv)
                                aimAccumY = aimAccumY + (dy / sv)
                                local toMoveX = 0
                                local toMoveY = 0
                                if aimAccumX >= 1 then
                                    toMoveX = math.floor(aimAccumX)
                                    aimAccumX = aimAccumX - toMoveX
                                elseif aimAccumX <= -1 then
                                    toMoveX = math.ceil(aimAccumX)
                                    aimAccumX = aimAccumX - toMoveX
                                end
                                if aimAccumY >= 1 then
                                    toMoveY = math.floor(aimAccumY)
                                    aimAccumY = aimAccumY - toMoveY
                                elseif aimAccumY <= -1 then
                                    toMoveY = math.ceil(aimAccumY)
                                    aimAccumY = aimAccumY - toMoveY
                                end
                                if toMoveX ~= 0 or toMoveY ~= 0 then
                                    toMoveX = math.clamp(toMoveX, -150, 150)
                                    toMoveY = math.clamp(toMoveY, -150, 150)
                                    mousemoverel(toMoveX, toMoveY)
                                end
                            else
                                mousemoverel(dx, dy)
                            end
                        end
                    end
                end)
            end
            if drawCircle and drawEnabled then
                pcall(function()
                    local vs = cam.ViewportSize
                    drawCircle.Position = Vector2.new(vs.X * 0.5, vs.Y * 0.5)
                    drawCircle.Radius = fovMax
                    drawCircle.Visible = true
                end)
            elseif drawCircle then
                pcall(function() drawCircle.Visible = false end)
            end
        end)
    end

    local function stopLoop()
        safeDisconnect(loopConn)
        loopConn = nil
    end

    inputConnBegin = UserInputService.InputBegan:Connect(function(input, gameProcessed)
        if gameProcessed then return end
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            leftDown = true
            startLoop()
        end
        if input.UserInputType == Enum.UserInputType.Keyboard then
            local bound = GetConfig(KEY_CONFIG, "V")
            if bound and Enum.KeyCode[bound] and input.KeyCode == Enum.KeyCode[bound] then
                pcall(function()
                    local t = ToggleAPI[aimbotToggle]
                    if t and t.Get and t.Set then t.Set(not t.Get()) end
                end)
            end
        end
    end)

    inputConnEnd = UserInputService.InputEnded:Connect(function(input)
        if input.UserInputType == Enum.UserInputType.MouseButton1 then
            leftDown = false
            persistentTarget = nil
            if not leftDown then stopLoop() end
        end
    end)

    RegisterUnload(function()
        safeDisconnect(inputConnBegin)
        safeDisconnect(inputConnEnd)
        safeDisconnect(loopConn)
        if drawCircle and drawCircle.Remove then pcall(function() drawCircle:Remove() end) end
        if playerRemoveConn and playerRemoveConn.Disconnect then pcall(function() playerRemoveConn:Disconnect() end) end
        for p,_ in pairs(teammateCache) do teammateCache[p] = nil end
        if playerCharConns then
            for p, conn in pairs(playerCharConns) do
                pcall(function() if conn and conn.Disconnect then conn:Disconnect() end end)
                playerCharConns[p] = nil
            end
        end
    end)
    _G.RivalsCHT_Aimbot = _G.RivalsCHT_Aimbot or {}
    _G.RivalsCHT_Aimbot.Start = startLoop
    _G.RivalsCHT_Aimbot.Stop = stopLoop
    _G.RivalsCHT_Aimbot.ForceActive = false
    _G.RivalsCHT_Aimbot.IsEnabled = function()
        local api = ToggleAPI[aimbotToggle]
        return api and api.Get and api.Get()
    end
    -- Allow other parts of the script to trigger/release aim-lock programmatically
    _G.RivalsCHT_Aimbot.Trigger = function()
        -- validate persistent target is alive before triggering
        if persistentTarget and persistentTarget.model then
            if not _G.RivalsCHT_Aimbot.IsAlive(persistentTarget.model) then
                pcall(function() _G.RivalsCHT_Aimbot.ClearPersistentTarget() end)
                return
            end
        end
        _G.RivalsCHT_Aimbot.ForceActive = true
        pcall(function() _G.RivalsCHT_Aimbot.Start() end)
    end
    _G.RivalsCHT_Aimbot.Release = function()
        _G.RivalsCHT_Aimbot.ForceActive = false
        pcall(function() _G.RivalsCHT_Aimbot.Stop() end)
    end
    _G.RivalsCHT_Aimbot.GetPersistentTarget = function()
        return persistentTarget
    end
    _G.RivalsCHT_Aimbot.HasPersistentTarget = function()
        return persistentTarget ~= nil
    end
    _G.RivalsCHT_Aimbot.SetPersistentTarget = function(model)
        if not model then return end
        if not _G.RivalsCHT_Aimbot.IsAlive(model) then return end
        persistentTarget = { model = model, player = Players:GetPlayerFromCharacter(model), lastPos = (model:FindFirstChild("Head") and model:FindFirstChild("Head").Position) or (model.PrimaryPart and model.PrimaryPart.Position), t = tick() }
        if type(_G) == "table" and _G.RivalsCHT_Aimbot then _G.RivalsCHT_Aimbot.PersistentTarget = persistentTarget end
    end

    _G.RivalsCHT_Aimbot.IsAlive = function(target)
        local Players = game:GetService("Players")
        local ok, hum
        if type(target) == "string" then
            local pl = Players:FindFirstChild(target)
            if not pl or not pl.Character then return false end
            ok, hum = pcall(function() return pl.Character:FindFirstChildOfClass("Humanoid") end)
        elseif typeof(target) == "Instance" then
            if target:IsA("Player") or target:IsA("PlayerInstance") then
                local pl = target
                if pl.Character then ok, hum = pcall(function() return pl.Character:FindFirstChildOfClass("Humanoid") end) end
            elseif target:IsA("Model") then
                ok, hum = pcall(function() return target:FindFirstChildOfClass("Humanoid") end)
            else
                return false
            end
        elseif type(target) == "table" and target.model then
            ok, hum = pcall(function() return target.model:FindFirstChildOfClass("Humanoid") end)
        else
            return false
        end
        if not ok then return false end
        if not hum then return false end
        if hum.Health == nil then return true end
        return (hum.Health > 0)
    end
    _G.RivalsCHT_Aimbot.ClearPersistentTarget = function()
        persistentTarget = nil
        if type(_G) == "table" and _G.RivalsCHT_Aimbot then _G.RivalsCHT_Aimbot.PersistentTarget = nil end
    end
    -- ** Aim Assist API to get if humanoid head is in FOV
    _G.RivalsCHT_AimAssist = _G.RivalsCHT_AimAssist or {}
    _G.RivalsCHT_AimAssist.IsHeadInFOV = function(target)
        local headPos = nil
        local headInst = nil
        local Players = game:GetService("Players")
        if type(target) == "string" then
            local pl = Players:FindFirstChild(target)
            if pl and pl.Character then
                headInst = pl.Character:FindFirstChild("Head") or pl.Character:FindFirstChild("UpperTorso") or pl.Character:FindFirstChild("HumanoidRootPart")
                if headInst and headInst.Position then headPos = headInst.Position end
            end
        elseif typeof(target) == "Instance" then
            if target:IsA("Model") then
                headInst = target:FindFirstChild("Head") or target:FindFirstChild("UpperTorso") or target:FindFirstChild("HumanoidRootPart")
                if headInst and headInst.Position then headPos = headInst.Position end
            elseif target:IsA("BasePart") then
                headInst = target
                headPos = target.Position
            end
        elseif typeof(target) == "Vector3" then
            headPos = target
        end

        local cam = workspace.CurrentCamera
        if not cam or not headPos then return false, nil, nil, headInst end

        local p = cam:WorldToViewportPoint(headPos)
        if not p or p.Z <= 0 then return false, nil, nil, headInst end
        local vs = cam.ViewportSize
        local cx, cy = vs.X * 0.5, vs.Y * 0.5
        local dx = p.X - cx
        local dy = p.Y - cy
        local dist = math.sqrt(dx*dx + dy*dy)
        local inFov = (dist <= (fovMax or 0))
        return inFov, dist, Vector2.new(p.X, p.Y), headInst
    end
end

-- ** Aimbot Logic Ends Here ** --

---------------------------------------------------------------------------

-- ** Target Behind Walls Logic Starts Here ** 

targetBehindWallsEnabled = false
do
    local ok, tApi = pcall(function() return ToggleAPI[targetBehindWallsToggle] end)
    if ok and tApi then
        pcall(function() targetBehindWallsEnabled = tApi.Get and tApi.Get() or false end)
        local prev = tApi.OnToggle
        tApi.OnToggle = function(state)
            if prev then pcall(prev, state) end
            targetBehindWallsEnabled = not not state
        end
    end
end


-- ** Target Behind Walls Logic Ends Here ** --

---------------------------------------------------------------------------

-- ** Aim Lock Keybind Logic Starts Here ** --

do
    local KEY_CONFIG = "combat.aimLockKey"
    local targetKey = Enum.KeyCode.Q

    local function updateTargetKey()
        local saved = GetConfig(KEY_CONFIG, "Q")
        targetKey = (type(saved) == "string" and Enum.KeyCode[saved]) or Enum.KeyCode.Q
    end

    updateTargetKey()

    -- Watch for keybind changes from UI
    pcall(function()
        local api = KeybindAPI[aimLockKeybind]
        if api then
            local prevOnBind = api.OnBind
            api.OnBind = function(k)
                if prevOnBind then prevOnBind(k) end
                updateTargetKey()
            end
        end
    end)

    local aimLockDown = false
    local kbBegan, kbEnded

    kbBegan = UserInputService.InputBegan:Connect(function(input)
        if input.UserInputType ~= Enum.UserInputType.Keyboard then return end
        if input.KeyCode == targetKey then
            aimLockDown = true
            if _G.RivalsCHT_Aimbot then _G.RivalsCHT_Aimbot.ForceActive = true; _G.RivalsCHT_Aimbot.Start() end
        end
    end)

    kbEnded = UserInputService.InputEnded:Connect(function(input)
        if input.UserInputType ~= Enum.UserInputType.Keyboard then return end
        if input.KeyCode == targetKey then
            aimLockDown = false
            if _G.RivalsCHT_Aimbot then _G.RivalsCHT_Aimbot.ForceActive = false end
            local leftHeld = false
            pcall(function() leftHeld = UserInputService:IsMouseButtonPressed(Enum.UserInputType.MouseButton1) end)
            if not leftHeld then if _G.RivalsCHT_Aimbot then _G.RivalsCHT_Aimbot.Stop() end end
        end
    end)

    RegisterUnload(function()
        if kbBegan and kbBegan.Disconnect then pcall(function() kbBegan:Disconnect() end) end
        if kbEnded and kbEnded.Disconnect then pcall(function() kbEnded:Disconnect() end) end
    end)

end




-- ** Aim Lock Keybind Logic Ends Here ** --

---------------------------------------------------------------------------

-- ** Show Enemy Weapons Logic Starts Here **

do
    local LocalPlayer = Players.LocalPlayer
    local ViewModels = workspace:FindFirstChild("ViewModels")
    
    if not ViewModels then
        return
    end
    
    local isEnabled = false
    local labels = {}
    local lastUpdate = 0
    local updateInterval = 0.2
    local FirstPersonCache = nil
    local firstPersonCacheTime = 0

    local labelContainer = Instance.new("Frame")
    labelContainer.Name = "EnemyWeaponLabels"
    labelContainer.Size = UDim2.new(0, 200, 0, 140)
    labelContainer.Position = UDim2.new(1, -220, 0, 24)
    labelContainer.AnchorPoint = Vector2.new(0, 0)
    labelContainer.BackgroundColor3 = COLORS.panel
    labelContainer.BackgroundTransparency = 0.04
    labelContainer.Visible = false
    labelContainer.Parent = gui

    local containerCorner = Instance.new("UICorner") containerCorner.CornerRadius = UDim.new(0,6) containerCorner.Parent = labelContainer
    local containerStroke = Instance.new("UIStroke") containerStroke.Color = COLORS.divider containerStroke.Transparency = 0.8 containerStroke.Thickness = 1 containerStroke.Parent = labelContainer

    local layout = Instance.new("UIListLayout")
    layout.SortOrder = Enum.SortOrder.LayoutOrder
    layout.Padding = UDim.new(0, 6)
    layout.Parent = labelContainer
    
    local function extractWeaponName(modelName)
        local parts = string.split(modelName, " - ")
        if #parts >= 3 then
            return parts[3]
        elseif #parts >= 2 then
            return parts[2]
        end
        return modelName
    end

    local function normalizeWeaponName(rawName)
        if not rawName or type(rawName) ~= "string" then return rawName end
        local lname = string.lower(rawName)
        for norm, list in pairs(WeaponDefs or {}) do
            if type(list) == "table" then
                for _, alias in ipairs(list) do
                    if type(alias) == "string" and string.lower(alias) == lname then
                        return string.gsub(norm, "_", " ")
                    end
                end
            end
        end
        local key = string.gsub(rawName, " ", "_")
        if WeaponDefs and WeaponDefs[key] then
            return string.gsub(key, "_", " ")
        end
        return rawName
    end
    
    local function extractPlayerName(modelName)
        local parts = string.split(modelName, " - ")
        if #parts >= 1 then
            return parts[1]
        end
        return "Unknown"
    end
    
    local function createWeaponLabel(playerName)
        local label = Instance.new("TextLabel")
        label.Name = "WeaponLabel_" .. playerName
        label.Size = UDim2.new(1, 0, 0, 26)
        label.BackgroundColor3 = COLORS.panelAlt or Color3.fromRGB(18,18,18)
        label.BackgroundTransparency = 0
        label.Font = Enum.Font.GothamSemibold
        label.TextSize = 13
        label.TextColor3 = COLORS.text
        label.Text = ""
        label.TextXAlignment = Enum.TextXAlignment.Left
        label.LayoutOrder = #labelContainer:GetChildren()
        label.Visible = false
        label.Parent = labelContainer

        local corner = Instance.new("UICorner")
        corner.CornerRadius = UDim.new(0, 6)
        corner.Parent = label

        local padding = Instance.new("UIPadding")
        padding.PaddingLeft = UDim.new(0, 8)
        padding.PaddingRight = UDim.new(0, 8)
        padding.Parent = label

        local stroke = Instance.new("UIStroke")
        stroke.Color = COLORS.divider
        stroke.Thickness = 1
        stroke.Transparency = 0.85
        stroke.Parent = label

        return label
    end
    
    local function updateWeaponDisplay()
        if not isEnabled then return end
        
        local currentTime = tick()
        if currentTime - lastUpdate < updateInterval then return end
        lastUpdate = currentTime
        
        local activePlayers = {}
        
        for _, weapon in pairs(ViewModels:GetChildren()) do
            if weapon:IsA("Model") then
                local playerName = extractPlayerName(weapon.Name)
                local weaponName = extractWeaponName(weapon.Name)
                
                if playerName == LocalPlayer.Name then
                    continue
                end
                
                local player = Players:FindFirstChild(playerName)
                if not player then
                    continue
                end

                local isTeammate = false
                if _G and _G.RivalsCHT_TeamCheck and type(_G.RivalsCHT_TeamCheck.IsTeammate) == "function" then
                    pcall(function() isTeammate = _G.RivalsCHT_TeamCheck.IsTeammate(player) end)
                end
                if isTeammate then
                    continue
                end
                
                local displayName = normalizeWeaponName(weaponName)
                activePlayers[playerName] = displayName
                
                if not labels[playerName] then
                    labels[playerName] = createWeaponLabel(playerName)
                end
                
                local label = labels[playerName]
                label.Text = playerName .. " | " .. displayName
                label.Visible = true
            end
        end
        
        for playerName, label in pairs(labels) do
            if not activePlayers[playerName] then
                label.Visible = false
            end
        end
    end
    
    local function enableWeaponDisplay()
        if isEnabled then return end
        
        isEnabled = true
        labelContainer.Visible = true
        
        local updateConnection = RunService.Heartbeat:Connect(updateWeaponDisplay)
        
        _G.RivalsCHTUI.RegisterUnload(function()
            isEnabled = false
            labelContainer.Visible = false
            updateConnection:Disconnect()
            for _, label in pairs(labels) do
                label:Destroy()
            end
            labels = {}
            if labelContainer and labelContainer.Parent then
                labelContainer:Destroy()
            end
        end)
    end
    
    local function disableWeaponDisplay()
        if not isEnabled then return end
        
        isEnabled = false
        labelContainer.Visible = false
        for _, label in pairs(labels) do
            label.Visible = false
        end
    end
    
    local function onToggleChanged(state)
        if state then
            enableWeaponDisplay()
        else
            disableWeaponDisplay()
        end
    end
    
    local initialEnabled = GetConfig("visuals.showEnemyWeapons", false)
    onToggleChanged(initialEnabled)
    
    local toggleAPI = ToggleAPI[showEnemyWeaponsToggle]
    if toggleAPI then
        local prev = toggleAPI.OnToggle
        toggleAPI.OnToggle = function(state)
            if prev then pcall(prev, state) end
            onToggleChanged(state)
        end
    end

    local function GetEnemyHeldWeapon(playerOrName)
        local target = playerOrName
        if type(target) == "string" then target = Players:FindFirstChild(target) end
        if not target or target == LocalPlayer then return nil end
        for _, vm in ipairs(ViewModels:GetChildren()) do
            if vm:IsA("Model") then
                local pn = extractPlayerName(vm.Name)
                if pn == target.Name then
                    local raw = extractWeaponName(vm.Name)
                    local norm = normalizeWeaponName(raw)
                    return norm, raw, vm
                end
            end
        end
        return nil
    end

    local function GetAllEnemyHeldWeapons()
        local out = {}
        for _, vm in ipairs(ViewModels:GetChildren()) do
            if vm:IsA("Model") then
                local pn = extractPlayerName(vm.Name)
                local raw = extractWeaponName(vm.Name)
                out[pn] = { Normalized = normalizeWeaponName(raw), Raw = raw }
            end
        end
        return out
    end

    local function GetLocalPlayerHeldWeapon()
        if not LocalPlayer then return nil end
        local now = tick()
        if not FirstPersonCache or (now - firstPersonCacheTime) > 0.2 then
            FirstPersonCache = ViewModels:FindFirstChild("FirstPerson")
            firstPersonCacheTime = now
        end
        if not FirstPersonCache then return nil end
        for _, child in ipairs(FirstPersonCache:GetChildren()) do
            if child:IsA("Model") then
                local raw = extractWeaponName(child.Name)
                local norm = normalizeWeaponName(raw)
                return norm, raw, child
            end
        end
        return nil
    end

    pcall(function()
        if type(_G) == "table" and _G.RivalsCHTUI then
            _G.RivalsCHTUI.ShowEnemyWeapons = _G.RivalsCHTUI.ShowEnemyWeapons or {}
            _G.RivalsCHTUI.ShowEnemyWeapons.GetEnemyHeldWeapon = GetEnemyHeldWeapon
            _G.RivalsCHTUI.ShowEnemyWeapons.GetAllEnemyHeldWeapons = GetAllEnemyHeldWeapons
            _G.RivalsCHTUI.ShowEnemyWeapons.GetLocalPlayerHeldWeapon = GetLocalPlayerHeldWeapon
        end
    end)
end

-- ** Show Enemy Weapons Logic Ends Here ** --

---------------------------------------------------------------------------

-- ** Sixth Sense Logic Starts Here ** --

-- ** Subspace Tripmine Detection

do
    local labels = {}
    local labelCount = 0
    local childAddedConn, childRemovedConn, renderConn
    local displayName = ("Subspace_Tripmine"):gsub("_"," ")
    local MAX_LABELS = 50
    local MAX_DIST = 300

    local function isTripminePart(part)
        if not part or not part:IsA("BasePart") then return false end
        local vm = Workspace:FindFirstChild("ViewModels")
        if vm and part:IsDescendantOf(vm) then return false end
        local cam = Workspace.CurrentCamera
        if cam and part:IsDescendantOf(cam) then return false end
        local name = string.lower(part.Name or "")
        if string.find(name, "tripmine") then return true end
        local anc = part:FindFirstAncestorOfClass("Model")
        if anc and string.find(string.lower(anc.Name or ""), "tripmine") then return true end
        return false
    end

    local function makeLabel(part)
        if labels[part] then return end
        if labelCount >= MAX_LABELS then return end
        if localPlayer and localPlayer.Character and part:IsDescendantOf(localPlayer.Character) then return end
        local cam = Workspace.CurrentCamera
        if cam and (part.Position - cam.CFrame.Position).Magnitude > MAX_DIST then return end

        local txt = Drawing.new("Text")
        part:SetAttribute("Rivals_Trap", true)
        part:SetAttribute("Rivals_TrapName", displayName)
        txt.Text = displayName
        txt.Size = 18
        txt.Color = (COLORS and COLORS.accent) or Color3.fromRGB(255,120,120)
        txt.Center = true
        txt.Outline = true
        txt.Visible = false
        labels[part] = txt
        labelCount = labelCount + 1
    end

    local function removeLabel(part)
        local d = labels[part]
        if not d then return end
        if d.Remove then d:Remove() end
        labels[part] = nil
        labelCount = labelCount - 1
        if part.SetAttribute then
            part:SetAttribute("Rivals_Trap", nil)
            part:SetAttribute("Rivals_TrapName", nil)
        end
    end

    local function scanAndCreate()
        local cam = Workspace.CurrentCamera
        local camPos = cam and cam.CFrame.Position or nil
        for _, obj in ipairs(Workspace:GetDescendants()) do
            if labelCount >= MAX_LABELS then break end
            if obj:IsA("BasePart") and isTripminePart(obj) then
                if not (camPos and (obj.Position - camPos).Magnitude > MAX_DIST) then
                    makeLabel(obj)
                end
            end
        end
    end

    local function onDescendantAdded(desc)
        local cam = Workspace.CurrentCamera
        local camPos = cam and cam.CFrame.Position or nil
        if desc:IsA("BasePart") then
            if isTripminePart(desc) and not (camPos and (desc.Position - camPos).Magnitude > MAX_DIST) then makeLabel(desc) end
        else
            for _, d in ipairs(desc:GetDescendants()) do
                if labelCount >= MAX_LABELS then break end
                if d:IsA("BasePart") and isTripminePart(d) and not (camPos and (d.Position - camPos).Magnitude > MAX_DIST) then
                    makeLabel(d)
                end
            end
        end
    end

    local function onDescendantRemoving(desc)
        if desc:IsA("BasePart") then
            removeLabel(desc)
        else
            for _, d in ipairs(desc:GetDescendants()) do
                if d:IsA("BasePart") then removeLabel(d) end
            end
        end
    end

    local function enable()
        if renderConn then return end
        scanAndCreate()
        childAddedConn = Workspace.DescendantAdded:Connect(onDescendantAdded)
        if Workspace.DescendantRemoving then childRemovedConn = Workspace.DescendantRemoving:Connect(onDescendantRemoving) end
        renderConn = RunService.RenderStepped:Connect(function()
            local cam = Workspace.CurrentCamera
            if not cam then
                for _, d in pairs(labels) do d.Visible = false end
                return
            end
            local camPos = cam.CFrame.Position
            for part, draw in pairs(labels) do
                if not part or not part.Parent then
                    removeLabel(part)
                else
                    local p, onScreen = cam:WorldToViewportPoint(part.Position)
                    if not onScreen or p.Z <= 0 or (part.Position - camPos).Magnitude > MAX_DIST then
                        draw.Visible = false
                    else
                        local dist = (part.Position - camPos).Magnitude
                        local ratio = math.clamp(50 / math.max(dist, 1), 0.125, 1)
                        draw.Size = math.floor(math.clamp(math.floor(32 * ratio), 12, 32))
                        draw.Position = Vector2.new(p.X, p.Y)
                        draw.Visible = true
                    end
                end
            end
        end)
    end

    local function disable()
        if renderConn then renderConn:Disconnect() renderConn = nil end
        if childAddedConn then childAddedConn:Disconnect() childAddedConn = nil end
        if childRemovedConn then childRemovedConn:Disconnect() childRemovedConn = nil end
        for p, _ in pairs(labels) do removeLabel(p) end
        labels = {}
        labelCount = 0
    end

    if GetConfig("combat.sixthSense", false) then enable() end

    local api = ToggleAPI and ToggleAPI[sixthSenseToggle]
    if api then
        local prev = api.OnToggle
        api.OnToggle = function(state)
            if prev then prev(state) end
            if state then enable() else disable() end
        end
        api.Set(GetConfig("combat.sixthSense", false))
    end

    if type(_G) == "table" and _G.RivalsCHTUI and type(_G.RivalsCHTUI.RegisterUnload) == "function" then
        _G.RivalsCHTUI.RegisterUnload(disable)
    else
        RegisterUnload(disable)
    end
end

-- ** Katana Detection

do
    local api = (_G and _G.RivalsCHTUI and _G.RivalsCHTUI.ShowEnemyWeapons) or nil
    local katanaDraw = nil
    local katanaConn = nil
    local katanaExpiry = 0
    local KATANA_DIST = 150
    local KATANA_TIME = 1.4

    local function removeKatanaDraw()
        if katanaDraw and katanaDraw.Remove then
            pcall(function() katanaDraw:Remove() end)
        end
        katanaDraw = nil
    end

    local function showKatanaMessage()
        if typeof(Drawing) ~= "table" or not Drawing.new then return end
        if not katanaDraw then
            katanaDraw = Drawing.new("Text")
            katanaDraw.Text = "Enemy is holding Katana!"
            katanaDraw.Color = (COLORS and COLORS.accent) or Color3.fromRGB(255,80,80)
            katanaDraw.Size = 25
            katanaDraw.Center = true
            katanaDraw.Outline = true
            local cam = Workspace.CurrentCamera
            if cam then
                local vs = cam.ViewportSize
                katanaDraw.Position = Vector2.new(vs.X * 0.5, 48)
            else
                katanaDraw.Position = Vector2.new(400, 48)
            end
        end
        katanaDraw.Visible = true
        katanaExpiry = tick() + KATANA_TIME
    end

    local function checkAndHideKatana()
        if katanaDraw and tick() > katanaExpiry then
            pcall(function() katanaDraw.Visible = false end)
        end
    end

    local function detectKatana()
        if not GetConfig("combat.sixthSense", false) then
            if katanaDraw then pcall(function() katanaDraw.Visible = false end) end
            return
        end

        local lp = Players.LocalPlayer
        local lpchar = lp and lp.Character
        local lpRoot = lpchar and (lpchar.PrimaryPart or lpchar:FindFirstChild("HumanoidRootPart"))
        if not lpRoot then
            checkAndHideKatana()
            return
        end

        for _, pl in ipairs(Players:GetPlayers()) do
            if pl ~= lp then
                local isTeam = false
                if _G and _G.RivalsCHT_TeamCheck and type(_G.RivalsCHT_TeamCheck.IsTeammate) == "function" then
                    pcall(function() isTeam = _G.RivalsCHT_TeamCheck.IsTeammate(pl) end)
                end
                if isTeam then continue end

                local norm, raw, vm
                if api and type(api.GetEnemyHeldWeapon) == "function" then
                    norm, raw, vm = api.GetEnemyHeldWeapon(pl)
                else
                    for _, m in ipairs((workspace:FindFirstChild("ViewModels") or {}):GetChildren()) do
                        if m:IsA("Model") then
                            local pn = string.split(m.Name, " - ")[1]
                            if pn == pl.Name then
                                raw = (function()
                                    local parts = string.split(m.Name, " - ")
                                    if #parts >= 3 then return parts[3] elseif #parts >= 2 then return parts[2] end
                                    return m.Name
                                end)()
                                norm = raw
                                vm = m
                                break
                            end
                        end
                    end
                end

                if raw and (string.find(string.lower(raw), "katana") or (norm and string.find(string.lower(tostring(norm)), "katana"))) then
                    local ch = pl.Character
                    if ch and ch.Parent then
                        local hrp = ch.PrimaryPart or ch:FindFirstChild("HumanoidRootPart")
                        if hrp and (lpRoot.Position - hrp.Position).Magnitude <= KATANA_DIST then
                            local dir = (lpRoot.Position - hrp.Position)
                            if dir.Magnitude > 0 then
                                local dirUnit = dir.Unit
                                local forward = hrp.CFrame.LookVector
                                local dot = forward:Dot(dirUnit)
                                if dot >= 0.3 then
                                    showKatanaMessage()
                                    return
                                end
                            end
                        end
                    end
                end
            end
        end

        checkAndHideKatana()
    end

    katanaConn = RunService.Heartbeat:Connect(detectKatana)

    local function cleanupKatana()
        if katanaConn and katanaConn.Disconnect then pcall(function() katanaConn:Disconnect() end) end
        removeKatanaDraw()
    end

    if type(_G) == "table" and _G.RivalsCHTUI and type(_G.RivalsCHTUI.RegisterUnload) == "function" then
        _G.RivalsCHTUI.RegisterUnload(cleanupKatana)
    else
        RegisterUnload(cleanupKatana)
    end
end


-- ** Sixth Sense Logic Ends Here ** --

---------------------------------------------------------------------------

-- ** Debug Config Logic Starts Here ** --
do
    local MAX_LINES = 8
    local REFRESH_RATE = 1.5 -- seconds
    local collapse = {}
    local buffer = {}
    local lastUpdate = 0
    local visible = false
    local drawBg = nil
    local drawText = nil
    local posConn = nil
    local SHIFT_LEFT = 400
    
    local hbConn

    local function fmt(v)
        if v == nil then return "nil" end
        if type(v) == "boolean" then return (v and "on" or "off") end
        if type(v) == "string" then return v end
        if type(v) == "number" then return tostring(v) end
        if typeof and typeof(v) == "EnumItem" then return v.Name end
        return tostring(v)
    end

    local keys = {}
    local function isPrimitive(val)
        local t = type(val)
        if t == "boolean" or t == "number" or t == "string" then return true end
        if typeof and typeof(val) == "EnumItem" then return true end
        return false
    end

    local function humanize(key)
        local lbl = tostring(key):gsub("[_%./]", " ")
        lbl = lbl:gsub("%s+", " ")
        lbl = lbl:gsub("^%l", string.upper)
        return lbl
    end

    local function flatten(tbl, prefix)
        prefix = prefix or ""
        if type(tbl) ~= "table" then return end
        for k,v in pairs(tbl) do
            local full = (prefix == "") and tostring(k) or (prefix .. "." .. tostring(k))
            if isPrimitive(v) then
                table.insert(keys, { key = full, label = humanize(full), cfg = v })
            elseif type(v) == "table" then
                flatten(v, full)
            end
        end
    end

    flatten(Config)
    MAX_LINES = math.min(32, math.max(8, #keys))

    local function getRuntime(entry)
        local function tryTable(t)
            if type(t) ~= "table" then return nil end
            for _, api in pairs(t) do
                if type(api) == "table" and api.Get and type(api.Get) == "function" then
                    local v = api.Get()
                    if entry.cfg == nil then
                        if v ~= nil then return v end
                    else
                        if type(v) == type(entry.cfg) then return v end
                    end
                end
            end
            return nil
        end

        local v = tryTable(ToggleAPI)
        if v == nil then v = tryTable(SliderAPI) end
        if v == nil then v = tryTable(KeybindAPI) end
        return v
    end

    local function scanConfig()
        local out = {}
        for _, e in ipairs(keys) do
            local cfgv = e.cfg
            if GetConfig then cfgv = GetConfig(e.key, nil) end
            local runtime = getRuntime(e)
            table.insert(out, {label = e.label, key = e.key, cfg = cfgv, runtime = runtime})
        end
        return out
    end

    local function makeUI()
        drawBg = Drawing.new("Square")
        drawBg.Filled = true
        drawBg.Color = COLORS.panel
        drawBg.Transparency = 0.04
        drawBg.Size = Vector2.new(320, 24 + MAX_LINES * 18)
        drawBg.Visible = false
        drawBg.ZIndex = 9998

        drawText = Drawing.new("Text")
        drawText.Size = 14
        drawText.Color = COLORS.text
        drawText.Outline = true
        drawText.Center = false
        drawText.Text = ""
        drawText.Visible = false
        drawText.ZIndex = 9999

        posConn = RunService.RenderStepped:Connect(function()
            local cam = workspace.CurrentCamera
            if cam then
                local vs = cam.ViewportSize
                local margin = 8
                local desiredW = 320
                local availW = math.max(64, vs.X - margin*2)
                local w = math.min(desiredW, availW)
                drawBg.Size = Vector2.new(w, drawBg.Size.Y)
                local x = vs.X - margin - w - SHIFT_LEFT
                local y = margin
                if x < 0 then x = margin end
                drawBg.Position = Vector2.new(x, y)
                drawText.Position = Vector2.new(x + 8, y + 2)
            else
                drawBg.Position = Vector2.new(400, 8)
                drawText.Position = Vector2.new(408, 10)
            end
        end)
    end

    local function push(msg)
        if not msg then return end
        if buffer[#buffer] == msg then
            collapse[msg] = (collapse[msg] or 1) + 1
        else
            table.insert(buffer, msg)
        end
        while #buffer > MAX_LINES do table.remove(buffer, 1) end
    end

    local function buildMessages(list)
        for _, v in ipairs(list) do
            local cfgv = v.cfg
            local runv = v.runtime

            if cfgv == nil and runv == nil then

            elseif cfgv == nil and runv ~= nil then
                push(string.format("%s not present in config; runtime is %s", v.label, fmt(runv)))

            else
                if runv == nil then
                    push(string.format("%s is %s in config, but not present at runtime", v.label, fmt(cfgv)))
                else
                    local same = false
                    if type(cfgv) == type(runv) and cfgv == runv then
                        same = true
                    else
                        if tostring(cfgv) == tostring(runv) then same = true end
                    end

                    if same then
                        push(string.format("%s is %s in config and runtime (ok)", v.label, fmt(cfgv)))
                    else
                        push(string.format("%s is %s in config, but runtime is %s ; config didn't apply", v.label, fmt(cfgv), fmt(runv)))
                    end
                end
            end
        end
    end

    local function render()
        if not drawText then return end
        local lines = {}
        for i, s in ipairs(buffer) do
            local cnt = collapse[s]
            if cnt and cnt > 1 then
                s = string.format("%s  (x%d)", s, cnt)
            end
            table.insert(lines, s)
        end
        local text = (#lines > 0) and table.concat(lines, "\n") or ""
        drawText.Text = text
        drawText.Visible = text ~= ""
        drawBg.Visible = drawText.Visible
    end

    local function refresh()
        local now = tick()
        if now - lastUpdate < REFRESH_RATE then return end
        lastUpdate = now
        collapse = {}
        buffer = {}
        local list = scanConfig()
        buildMessages(list)
        render()
    end

    local function show(b)
        if b and not drawText then makeUI() end
        if drawText then drawText.Visible = b end
        if drawBg then drawBg.Visible = b end
        visible = b
        if b and not hbConn then
            hbConn = RunService.Heartbeat:Connect(function()
                refresh()
            end)
        elseif not b and hbConn then
            hbConn:Disconnect()
            hbConn = nil
        end
    end

    do
        local foundApi = nil
        if ToggleAPI then
            for frame, api in pairs(ToggleAPI) do
                if frame and type(frame) == "userdata" and frame:IsA("Frame") then
                    for _, child in ipairs(frame:GetChildren()) do
                        if child:IsA("TextLabel") and child.Text == "Debug Config" then
                            foundApi = api
                            break
                        end
                    end
                end
                if foundApi then break end
            end
        end

        if foundApi then
            local prev = foundApi.OnToggle
            foundApi.OnToggle = function(state)
                if prev then prev(state) end
                show(not not state)
            end
            if foundApi.Get and foundApi.Get() then show(true) else show(false) end
        else
            if GetConfig and GetConfig("settings.debugConfig", false) then show(true) end
        end
    end

    RegisterUnload(function()
        if hbConn and hbConn.Disconnect then hbConn:Disconnect() end
        if posConn and posConn.Disconnect then posConn:Disconnect() end
        if drawText and drawText.Remove then drawText:Remove() end
        if drawBg and drawBg.Remove then drawBg:Remove() end
    end)
end

-- ** Debug Config Logic Ends Here ** --

---------------------------------------------------------------------------

-- ** Auto Shoot Logic Starts Here ** --

do
    local RunService = game:GetService("RunService")
    local Players = game:GetService("Players")
    local UserInputService = game:GetService("UserInputService")

    local autoConn = nil
    local firing = false
    local lastInFov = 0
    local FOV_MISS_TIMEOUT = 1.5

    local KEY_CONFIG = "combat.enableAutoShootKey"
    local keyConn = nil
    
    local debugLabel = makeDebugLabel("AutoShoot: OFF")
    local lastDebugMsg = nil
    local persistentEngaged = false
    local katanaBlocked = false

    -- ** Aimlock keybind config
    do
        local api = KeybindAPI[aimLockKeybind]
        local saved = GetConfig("combat.aimLockKey", "Q")
        if api and type(saved) == "string" and Enum.KeyCode[saved] then
            api.Set(Enum.KeyCode[saved])
        end
        if api then
            api.OnBind = function(k)
                local name = nil
                if typeof(k) == "EnumItem" then name = k.Name elseif type(k) == "string" then name = tostring(k) end
                if name then SetConfig("combat.aimLockKey", name) end
            end
        end
    end

    do
        local keyApi = KeybindAPI[enableAutoShootKeybind]
        local saved = GetConfig(KEY_CONFIG, "Y")
        pcall(function()
            if keyApi and type(saved) == "string" and Enum.KeyCode[saved] then keyApi.Set(Enum.KeyCode[saved]) end
        end)
        if keyApi then
            keyApi.OnBind = function(k)
                local name = nil
                if typeof(k) == "EnumItem" then name = k.Name elseif type(k) == "string" then name = tostring(k) end
                if name then SetConfig(KEY_CONFIG, name) end
            end
        end
    end

    local function isHeadInFOV(headInst)
        if not headInst or not headInst.Position then return false, nil end
        local cam = workspace.CurrentCamera
        if not cam then return false, nil end
        local p = cam:WorldToViewportPoint(headInst.Position)
        if not p or p.Z <= 0 then return false, nil end
        local vs = cam.ViewportSize
        local cx, cy = vs.X * 0.5, vs.Y * 0.5
        local dx = p.X - cx
        local dy = p.Y - cy
        local dist = math.sqrt(dx*dx + dy*dy)
        local fovRadius = GetConfig("combat.aimbotFOV", 700) or 700
        return (dist <= fovRadius), dist
    end

    local function isHoldingKatana(playerOrNil)
        if not playerOrNil then return false end
        if type(_G) == "table" and _G.RivalsCHTUI and _G.RivalsCHTUI.ShowEnemyWeapons and type(_G.RivalsCHTUI.ShowEnemyWeapons.GetEnemyHeldWeapon) == "function" then
            local ok, norm, raw = pcall(function() return _G.RivalsCHTUI.ShowEnemyWeapons.GetEnemyHeldWeapon(playerOrNil) end)
            if ok and (type(norm) == "string" or type(raw) == "string") then
                local sraw = (type(raw) == "string") and string.lower(raw) or ""
                local snorm = (type(norm) == "string") and string.lower(norm) or ""
                if string.find(sraw, "katana") or string.find(snorm, "katana") then
                    return true
                end
            end
        end

        return false
    end

    local function isHoldingRiotShield(playerOrNil)
        if not playerOrNil or playerOrNil == Players.LocalPlayer then
            if type(_G) == "table" and _G.RivalsCHTUI and _G.RivalsCHTUI.ShowEnemyWeapons and type(_G.RivalsCHTUI.ShowEnemyWeapons.GetLocalPlayerHeldWeapon) == "function" then
                local ok, norm, raw = pcall(function() return _G.RivalsCHTUI.ShowEnemyWeapons.GetLocalPlayerHeldWeapon() end)
                if ok then
                    local sraw = (type(raw) == "string") and string.lower(raw) or ""
                    local snorm = (type(norm) == "string") and string.lower(norm) or ""
                    if string.find(sraw, "riot shield") or string.find(snorm, "riot shield") or string.find(sraw, "shield") or string.find(snorm, "shield") then
                        return true
                    end
                end
            end
        else
            if type(_G) == "table" and _G.RivalsCHTUI and _G.RivalsCHTUI.ShowEnemyWeapons and type(_G.RivalsCHTUI.ShowEnemyWeapons.GetEnemyHeldWeapon) == "function" then
                local ok, norm, raw = pcall(function() return _G.RivalsCHTUI.ShowEnemyWeapons.GetEnemyHeldWeapon(playerOrNil) end)
                if ok then
                    local sraw = (type(raw) == "string") and string.lower(raw) or ""
                    local snorm = (type(norm) == "string") and string.lower(norm) or ""
                    if string.find(sraw, "riot shield") or string.find(snorm, "riot shield") or string.find(sraw, "shield") or string.find(snorm, "shield") then
                        return true
                    end
                end
            end
        end
        return false
    end

    local function isVisibleToCamera(headInst)
        if not headInst or not headInst.Parent then return false end
        local cam = workspace.CurrentCamera
        if not cam then return false end
        
        local rp = RaycastParams.new()
        rp.FilterType = Enum.RaycastFilterType.Blacklist
        rp.FilterDescendantsInstances = {headInst.Parent}
        
        local origin = cam.CFrame.Position
        local direction = headInst.Position - origin
        local ray = workspace:Raycast(origin, direction, rp)
        
        if ray and ray.Instance and not ray.Instance:IsDescendantOf(headInst.Parent) then
            return false
        end
        
        return true
    end

    local function checkAndFire()
        local autoShootEnabled = GetConfig("combat.autoShoot", false)
        if not autoShootEnabled then 
            if firing then
                mouse1release()
                firing = false
                if _G and _G.RivalsCHT_Aimbot then
                    _G.RivalsCHT_Aimbot.ForceActive = false
                    _G.RivalsCHT_Aimbot.Stop()
                end
            end
            if debugLabel then debugLabel.Set("AutoShoot: OFF (disabled)") end
            return 
        end
        
        local debugMsg = "AutoShoot: Scanning..."
        local found = nil
        local persistentEnabled = GetConfig("combat.persistentAimbot", false)

        if not found then
            for _, pl in ipairs(Players:GetPlayers()) do
                if pl ~= Players.LocalPlayer then
                    local isTeam = false
                    if _G and _G.RivalsCHT_TeamCheck and type(_G.RivalsCHT_TeamCheck.IsTeammate) == "function" then
                        isTeam = _G.RivalsCHT_TeamCheck.IsTeammate(pl)
                    end
                    if isTeam then continue end

                    local char = pl.Character
                    if not char then continue end
                    local okAlive, aliveRes = pcall(function() return _G.RivalsCHT_Aimbot.IsAlive(char) end)
                    if not okAlive or not aliveRes then continue end

                    local headInst = char:FindFirstChild("Head") or char:FindFirstChild("UpperTorso") or char:FindFirstChild("HumanoidRootPart")
                    if not headInst then continue end

                    local inFov, screenDist = isHeadInFOV(headInst)

                    if inFov then
                        debugMsg = "AutoShoot: " .. pl.Name .. " in FOV (dist=" .. tostring(math.floor(screenDist or 0)) .. ")"
                        local isVis = isVisibleToCamera(headInst)
                        if isVis then
                            found = {player = pl, head = headInst}
                            debugMsg = "AutoShoot: TARGET LOCKED - " .. pl.Name
                            break
                        else
                            debugMsg = "AutoShoot: " .. pl.Name .. " blocked by geometry"
                        end
                    end
                end
            end
        end

        if not found then
            local persistentEnabled = GetConfig("combat.persistentAimbot", false)
            if persistentEnabled and _G and _G.RivalsCHT_Aimbot and type(_G.RivalsCHT_Aimbot.GetPersistentTarget) == "function" then
                local pt = _G.RivalsCHT_Aimbot.GetPersistentTarget()
                if pt and pt.model and pt.model.Parent then
                    local headInst = pt.model:FindFirstChild("Head") or pt.model:FindFirstChild("UpperTorso") or pt.model:FindFirstChild("HumanoidRootPart")
                    if headInst and headInst.Position then
                        local pl = pt.player or Players:GetPlayerFromCharacter(pt.model)
                        local isTeam = false
                        if pl and _G and _G.RivalsCHT_TeamCheck and type(_G.RivalsCHT_TeamCheck.IsTeammate) == "function" then
                            isTeam = _G.RivalsCHT_TeamCheck.IsTeammate(pl)
                        end
                        if not isTeam then
                            local isVis = isVisibleToCamera(headInst)
                            if isVis then
                                found = {player = pl, head = headInst}
                                debugMsg = "AutoShoot: PERSISTENT TARGET LOCKED (out of FOV)"
                                if persistentEnabled and _G and _G.RivalsCHT_Aimbot and not persistentEngaged then
                                    persistentEngaged = true
                                    if type(_G.RivalsCHT_Aimbot.SetPersistentTarget) == "function" then
                                        _G.RivalsCHT_Aimbot.SetPersistentTarget(headInst.Parent)
                                    end
                                    if type(_G.RivalsCHT_Aimbot.Trigger) == "function" then
                                        _G.RivalsCHT_Aimbot.Trigger()
                                    end
                                end
                            else
                                if firing then
                                    mouse1release()
                                    firing = false
                                    if _G and _G.RivalsCHT_Aimbot then
                                        _G.RivalsCHT_Aimbot.ForceActive = false
                                        _G.RivalsCHT_Aimbot.Stop()
                                    end
                                end
                                debugMsg = "AutoShoot: Persistent target blocked by geometry"
                            end
                        else
                            debugMsg = "AutoShoot: Persistent target is teammate"
                        end
                    end
                end
            end
        end

        if found then
            lastInFov = tick()
            local localPlayerHasShield = false
            pcall(function() localPlayerHasShield = isHoldingRiotShield(Players.LocalPlayer) end)

            local holdingKat = false
            local holdingShield = false
            pcall(function() holdingKat = isHoldingKatana(found.player) end)
            pcall(function() holdingShield = isHoldingRiotShield(found.player) end)

            local holdingSpray = false
            pcall(function()
                if type(_G) == "table" and _G.RivalsCHTUI and _G.RivalsCHTUI.ShowEnemyWeapons and type(_G.RivalsCHTUI.ShowEnemyWeapons.GetLocalPlayerHeldWeapon) == "function" then
                    local ok, norm, raw = pcall(function() return _G.RivalsCHTUI.ShowEnemyWeapons.GetLocalPlayerHeldWeapon() end)
                    if ok and norm and string.find(string.lower(norm), "spray") then
                        holdingSpray = true
                    end
                end
            end)

            local shouldBlock = (holdingKat or holdingShield or localPlayerHasShield) and not holdingSpray

            if shouldBlock then
                katanaBlocked = true
                local reason = ""
                if localPlayerHasShield then
                    reason = "Local player has Riot Shield"
                elseif holdingShield then
                    reason = "Target has Riot Shield"
                else
                    reason = "Target has Katana"
                end
                debugMsg = "AutoShoot: " .. reason .. " — Aim lock only: " .. (found.player and found.player.Name or "unknown")
                -- ** now trigger aimlock
                if _G and _G.RivalsCHT_Aimbot then
                    if type(_G.RivalsCHT_Aimbot.SetPersistentTarget) == "function" then
                        pcall(function() _G.RivalsCHT_Aimbot.SetPersistentTarget(found.head.Parent) end)
                        persistentEngaged = true
                    end
                    if type(_G.RivalsCHT_Aimbot.Trigger) == "function" then
                        pcall(_G.RivalsCHT_Aimbot.Trigger)
                    else
                        _G.RivalsCHT_Aimbot.ForceActive = true
                        pcall(function() _G.RivalsCHT_Aimbot.Start() end)
                    end
                end
                if firing then mouse1release(); firing = false end
            else
                katanaBlocked = false
                if not firing then
                    firing = true
                    debugMsg = "AutoShoot: FIRING at " .. found.player.Name
                    -- ** always trigger aimlock when firing
                    if _G and _G.RivalsCHT_Aimbot then
                        if type(_G.RivalsCHT_Aimbot.SetPersistentTarget) == "function" then
                            pcall(function() _G.RivalsCHT_Aimbot.SetPersistentTarget(found.head.Parent) end)
                            persistentEngaged = true
                        end
                        if type(_G.RivalsCHT_Aimbot.Trigger) == "function" then
                            pcall(_G.RivalsCHT_Aimbot.Trigger)
                        else
                            _G.RivalsCHT_Aimbot.ForceActive = true
                            _G.RivalsCHT_Aimbot.Start()
                        end
                    end
                    mouse1press()
                else
                    debugMsg = "AutoShoot: Holding fire on " .. found.player.Name
                    -- ** always trigger aimlock when holding fire
                    if _G and _G.RivalsCHT_Aimbot then
                        if type(_G.RivalsCHT_Aimbot.SetPersistentTarget) == "function" then
                            pcall(function() _G.RivalsCHT_Aimbot.SetPersistentTarget(found.head.Parent) end)
                            persistentEngaged = true
                        end
                        if type(_G.RivalsCHT_Aimbot.Trigger) == "function" then
                            pcall(_G.RivalsCHT_Aimbot.Trigger)
                        else
                            _G.RivalsCHT_Aimbot.ForceActive = true
                            _G.RivalsCHT_Aimbot.Start()
                        end
                    end
                end
            end
        else
            if firing then
                mouse1release()
                firing = false
                debugMsg = "AutoShoot: Released fire"
                -- ** always clear aimlock when fire ends
                if _G and _G.RivalsCHT_Aimbot then
                    _G.RivalsCHT_Aimbot.ForceActive = false
                    _G.RivalsCHT_Aimbot.Stop()
                    if type(_G.RivalsCHT_Aimbot.ClearPersistentTarget) == "function" then
                        pcall(_G.RivalsCHT_Aimbot.ClearPersistentTarget)
                    end
                end
                if persistentEngaged then
                    persistentEngaged = false
                end
            else
                debugMsg = "AutoShoot: Waiting for target"
                -- ** always clear aimlock if not firing
                if _G and _G.RivalsCHT_Aimbot then
                    _G.RivalsCHT_Aimbot.ForceActive = false
                    _G.RivalsCHT_Aimbot.Stop()
                    if type(_G.RivalsCHT_Aimbot.ClearPersistentTarget) == "function" then
                        pcall(_G.RivalsCHT_Aimbot.ClearPersistentTarget)
                    end
                end
                if persistentEngaged then
                    persistentEngaged = false
                end
            end
        
        end
        if debugLabel and debugMsg ~= lastDebugMsg then debugLabel.Set(debugMsg) lastDebugMsg = debugMsg end
    end

    pcall(function()
        keyConn = UserInputService.InputBegan:Connect(function(input, gameProcessed)
            if gameProcessed then return end
            if input.UserInputType ~= Enum.UserInputType.Keyboard then return end
            local bound = GetConfig(KEY_CONFIG, "Y")
            local target = bound and Enum.KeyCode[bound]
            if target and input.KeyCode == target then
                local currentState = GetConfig("combat.autoShoot", false)
                local newState = not currentState
                SetConfig("combat.autoShoot", newState)
                local api = ToggleAPI[autoShootToggle]
                if api and api.Set then api.Set(newState) end
            end
        end)
    end)

    local api = ToggleAPI[autoShootToggle]
    if api then
        pcall(function()
            if api.Get and api.Get() and not autoConn then autoConn = RunService.Heartbeat:Connect(checkAndFire) end
        end)
        local p = api.OnToggle
        api.OnToggle = function(state)
            if type(p) == "function" then p(state) end
            makeNotification(state and "Auto-Shoot is ON" or "Auto-Shoot is OFF", 3, nil, "AutoShootToggle")
            if state then
                if not autoConn then autoConn = RunService.Heartbeat:Connect(checkAndFire) end
                if debugLabel then debugLabel.Set("AutoShoot: ON") end
            else
                if autoConn then autoConn:Disconnect() autoConn = nil end
                if firing then mouse1release() firing = false end
                if _G and _G.RivalsCHT_Aimbot then
                    _G.RivalsCHT_Aimbot.ForceActive = false
                    pcall(function() _G.RivalsCHT_Aimbot.Stop() end)
                    if type(_G.RivalsCHT_Aimbot.ClearPersistentTarget) == "function" then
                        pcall(_G.RivalsCHT_Aimbot.ClearPersistentTarget)
                    end
                end
                persistentEngaged = false
                if debugLabel then debugLabel.Set("AutoShoot: OFF") end
            end
        end
    end

    RegisterUnload(function()
        if autoConn and autoConn.Disconnect then autoConn:Disconnect() end
        if keyConn and keyConn.Disconnect then keyConn:Disconnect() end
        if firing then mouse1release() end
        if debugLabel and debugLabel.Destroy then debugLabel.Destroy() end
    end)
end

-- ** Auto Shoot Logic Ends Here ** --

---------------------------------------------------------------------------

-- =================== Very end of Your Desire =================== --

-- ** Like a wise man once said, "Show me the client's state, and I'll show you the perfect hook." - some guy lol ** --
