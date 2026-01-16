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
    btn.Size = UDim2.new(0, 120, 0, 32)
    btn.AutoButtonColor = false
    btn.Font = Enum.Font.GothamBold
    btn.TextSize = 18
    btn.Text = name
    btn.BackgroundColor3 = COLORS.bg
    btn.TextColor3 = COLORS.tabText
    btn.BorderSizePixel = 0

    -- ** active tab indicator
    local indicator = Instance.new("Frame")
    indicator.Name = "ActiveIndicator"
    indicator.Size = UDim2.new(1, -16, 0, 4)
    indicator.Position = UDim2.new(0, 8, 1, -10)
    indicator.BackgroundColor3 = COLORS.accent
    indicator.BackgroundTransparency = 1
    indicator.ZIndex = btn.ZIndex - 1
    local indCorner = Instance.new("UICorner") indCorner.CornerRadius = UDim.new(0, 4) indCorner.Parent = indicator
    indicator.Parent = btn

    local page = Instance.new("Frame")
    page.Name = name .. "Page"
    page.Size = UDim2.new(1,0,1,0)
    page.BackgroundTransparency = 1

    if tabsParent then btn.Parent = tabsParent end
    if pagesParent then page.Parent = pagesParent end

    if tabsParent then
        pcall(function()
            local btnWidth = 120
            local spacing = 8
            local idx = 0
            for _,c in ipairs(tabsParent:GetChildren()) do
                if c:IsA("TextButton") and c ~= btn then
                    idx = idx + 1
                end
            end
            btn.Position = UDim2.new(0, idx * (btnWidth + spacing), 0, 6)
        end)
    end


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

    -- ** Left col
    local leftCol = Instance.new("Frame")
    leftCol.Name = "LeftCol"
    leftCol.Size = UDim2.new(0.5, -12, 1, -12)
    leftCol.Position = UDim2.new(0, 8, 0, 8)
    leftCol.BackgroundColor3 = COLORS.panel
    leftCol.Parent = page
    leftCol.ClipsDescendants = true
    local list = Instance.new("UIListLayout") list.Parent = leftCol
    list.SortOrder = Enum.SortOrder.LayoutOrder

    -- ** Header for left col
    if colHeaders and colHeaders.Left then
        local hdr = Instance.new("TextLabel")
        hdr.Name = "Header"
        hdr.Size = UDim2.new(1, -12, 0, 24)
        hdr.Position = UDim2.new(0, 6, 0, 6)
        hdr.BackgroundTransparency = 1
        hdr.Font = Enum.Font.GothamBold
        hdr.TextSize = 16
        hdr.Text = tostring(colHeaders.Left)
        hdr.TextColor3 = COLORS.textDim
        hdr.TextXAlignment = Enum.TextXAlignment.Left
        hdr.LayoutOrder = 0
        hdr.Parent = leftCol
    end


    -- ** Right col
    local rightCol = Instance.new("Frame")
    rightCol.Name = "RightCol"
    rightCol.Size = UDim2.new(0.5, -12, 1, -12)
    rightCol.Position = UDim2.new(0.5, 8, 0, 8)
    rightCol.BackgroundColor3 = COLORS.panel
    rightCol.Parent = page
    rightCol.ClipsDescendants = true
    local list2 = Instance.new("UIListLayout") list2.Parent = rightCol
    list2.SortOrder = Enum.SortOrder.LayoutOrder

    -- ** Header for right col
    if colHeaders and colHeaders.Right then
        local hdrr = Instance.new("TextLabel")
        hdrr.Name = "Header"
        hdrr.Size = UDim2.new(1, -12, 0, 24)
        hdrr.Position = UDim2.new(0, 6, 0, 6)
        hdrr.BackgroundTransparency = 1
        hdrr.Font = Enum.Font.GothamBold
        hdrr.TextSize = 16
        hdrr.Text = tostring(colHeaders.Right)
        hdrr.TextColor3 = COLORS.textDim
        hdrr.TextXAlignment = Enum.TextXAlignment.Left
        hdrr.LayoutOrder = 0
        hdrr.Parent = rightCol
    end

    -- ** vertical divider between cols
    local divider = Instance.new("Frame")
    divider.Name = "Divider"
    divider.Size = UDim2.new(0, 2, 1, -16)
    divider.Position = UDim2.new(0.5, -1, 0, 8)
    divider.BackgroundColor3 = COLORS.divider
    divider.Parent = page
    local divCorner = Instance.new("UICorner") divCorner.CornerRadius = UDim.new(0,1) divCorner.Parent = divider

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
    local innerCorner = Instance.new("UICorner")
    innerCorner.CornerRadius = UDim.new(1, 0)
    innerCorner.Parent = inner

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

    local container = Instance.new("Frame")
    container.Name = "RivalsNotification"
    container.Size = UDim2.new(0, 420, 0, 56)
    container.BackgroundColor3 = COLORS.panelDark
    container.BorderSizePixel = 0
    container.ZIndex = holder.ZIndex
    container.LayoutOrder = math.floor(tick() * 1000)
    container.Parent = holder
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
    label.Font = Enum.Font.GothamBold
    label.TextSize = 16
    label.Text = tostring(text or "Notification")
    label.TextColor3 = COLORS.text
    label.TextStrokeTransparency = 0.7
    label.TextXAlignment = Enum.TextXAlignment.Left
    label.TextYAlignment = Enum.TextYAlignment.Center
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
        container.Size = UDim2.new(0, 420, 0, 0)
        container.Position = container.Position
        TweenService:Create(container, TweenInfo.new(0.34, Enum.EasingStyle.Elastic, Enum.EasingDirection.Out), {Size = UDim2.new(0, 420, 0, 56)}):Play()
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
    local barCorner = Instance.new("UICorner") barCorner.CornerRadius = UDim.new(0,6) barCorner.Parent = bar

    local fill = Instance.new("Frame")
    fill.Name = "Fill"
    fill.Size = UDim2.new(0, 0, 1, 0)
    fill.BackgroundColor3 = COLORS.accent
    fill.BorderSizePixel = 0
    fill.Parent = bar
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

    -- API
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
    local dispCorner = Instance.new("UICorner") dispCorner.CornerRadius = UDim.new(0,6) dispCorner.Parent = display
    local dispPad = Instance.new("UIPadding") dispPad.Parent = display; dispPad.PaddingLeft = UDim.new(0,8)

    local swatch = Instance.new("Frame")
    swatch.Size = UDim2.new(0, 20, 0, 20)
    swatch.Position = UDim2.new(0, 0, 0.5, -10)
    swatch.BackgroundColor3 = (type(defaultColor) == "Color3") and defaultColor or COLORS.accent
    swatch.BorderSizePixel = 0
    swatch.Parent = display
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

    local choice
    btnNo.MouseButton1Click:Connect(function() choice = false end)
    btnYes.MouseButton1Click:Connect(function() choice = true end)

    while choice == nil do wait() end
    if choice == false then
        if sg and sg.Parent then sg:Destroy() end
        return
    else
        if sg and sg.Parent then sg:Destroy() end
    end
end

-- ** Unsupported game check ends here **

-- Invoke now that `Config` is available and the function is defined
pcall(function() if type(showUnsupportedPopup) == "function" then showUnsupportedPopup() end end)

--------------------------------------------------------------------------

-- ** Build UI
local root = Instance.new("Frame")
root.Size = UDim2.new(0, 760, 0, 520)
root.Position = UDim2.new(0.5, -380, 0.5, -260)
root.AnchorPoint = Vector2.new(0.0,0.0)
root.BackgroundColor3 = COLORS.bg
root.Parent = gui
local rootCorner = Instance.new("UICorner") rootCorner.Parent = root

local tabsBar = Instance.new("Frame")
tabsBar.Size = UDim2.new(1,0,0,40)
tabsBar.Position = UDim2.new(0,0,0,0)
tabsBar.BackgroundTransparency = 1
tabsBar.Parent = root

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

local pages = Instance.new("Frame")
pages.Size = UDim2.new(1,0,1,-40)
pages.Position = UDim2.new(0,0,0,40)
pages.BackgroundTransparency = 1
pages.Parent = root

local tabsUnderlay = Instance.new("Frame")
tabsUnderlay.Name = "TabsUnderlay"
tabsUnderlay.Size = UDim2.new(1, -16, 0, 10)
tabsUnderlay.Position = UDim2.new(0, 8, 0, 40)
tabsUnderlay.BackgroundColor3 = COLORS.panel
tabsUnderlay.Parent = root
local tabsUnderCorner = Instance.new("UICorner") tabsUnderCorner.CornerRadius = UDim.new(0,4) tabsUnderCorner.Parent = tabsUnderlay
tabsUnderlay.ZIndex = 1
tabsBar.ZIndex = 2


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

---------------------------------------------------------------------------


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


-- ** Save Settings to Config **
BindToggleToConfig(showGuiOnLoadToggle, "settings.showGuiOnLoad", true)
BindToggleToConfig(autoScaleUIToggle, "settings.autoScaleUI", false)
BindToggleToConfig(warnIfUnsupportedGameToggle, "settings.warnIfUnsupportedGame", true)
BindToggleToConfig(showNotificationsToggle, "settings.enableNotifications", true)


---------------------------------------------------------------------------

-- ** Combat Tab Stuff

local aimbotToggle = makeToggle(combatTab.LeftCol, "Aimbot")
local enableAimbotKeybind = makeKeyBindButton(combatTab.LeftCol, "Enable Aimbot", Enum.KeyCode.V)

local initialSmoothing = GetConfig("combat.aimbotSmoothing", 1) or 1
local initialAimbotFOV = GetConfig("combat.aimbotFOV", 700) or 700

local useAimbotSmoothingToggle = makeToggle(combatTab.RightCol, "Use Aimbot Smoothing")
local smoothingSlider = makeSlider(combatTab.RightCol, "Aimbot Smooth", 1, 100, initialSmoothing)
local aimbotFOVSlider = makeSlider(combatTab.RightCol, "Aimbot FOV", 1, 1000, initialAimbotFOV)
local drawFovCircleToggle = makeToggle(combatTab.LeftCol, "Draw FOV Circle")
local targetBehindWallsToggle = makeToggle(combatTab.LeftCol, "Target Behind Walls")
local aimLockKeybind = makeKeyBindButton(combatTab.RightCol, "Aim Lock Keybind", Enum.KeyCode.Q)
local teamCheckToggle = makeToggle(combatTab.LeftCol, "Team Check")



-- ** Save Combat to Config **
BindToggleToConfig(aimbotToggle, "combat.aimbot", false)
BindToggleToConfig(useAimbotSmoothingToggle, "combat.useAimbotSmoothing", false)
BindToggleToConfig(drawFovCircleToggle, "combat.drawFovCircle", false)
BindToggleToConfig(targetBehindWallsToggle, "combat.targetBehindWalls", false)
BindToggleToConfig(teamCheckToggle, "combat.teamCheck", true)


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
    Katana = {
        "katana"
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
        local _did_skip = false
        pcall(function()
            local teamEnabled = false
            local ok, tApi = pcall(function() return ToggleAPI[teamCheckToggle] end)
            if ok and tApi and type(tApi.Get) == "function" then
                teamEnabled = not not tApi.Get()
            else
                teamEnabled = not not GetConfig("combat.teamCheck", true)
            end
            if teamEnabled and _G and _G.RivalsCHT_TeamCheck and type(_G.RivalsCHT_TeamCheck.IsTeammate) == "function" then
                local ok2, isTeam = pcall(function() return _G.RivalsCHT_TeamCheck.IsTeammate(p) end)
                if ok2 and isTeam then
                    _did_skip = true
                end
            end
        end)
        if _did_skip then return end
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
                local _skip = false
                pcall(function()
                    local teamEnabled = false
                    local ok, tApi = pcall(function() return ToggleAPI[teamCheckToggle] end)
                    if ok and tApi and type(tApi.Get) == "function" then
                        teamEnabled = not not tApi.Get()
                    else
                        teamEnabled = not not GetConfig("combat.teamCheck", true)
                    end
                    if teamEnabled and _G and _G.RivalsCHT_TeamCheck and type(_G.RivalsCHT_TeamCheck.IsTeammate) == "function" then
                        local ok2, isTeam = pcall(function() return _G.RivalsCHT_TeamCheck.IsTeammate(p) end)
                        if ok2 and isTeam then
                            _skip = true
                        end
                    end
                end)
                if _skip then
                    removeBoxForPlayer(p)
                    continue
                end
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
                        local ok2, lbl = pcall(function() return hrp:FindFirstChild("TeammateLabel") end)
                        local res = ok2 and lbl ~= nil
                        teammateCache[pl] = { hrp = hrp, isTeam = res }
                        return res
                    end
                end
                teammateCache[pl] = { hrp = nil, isTeam = false }
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
            end
        end
        if sApi then
            smoothingValue = sApi.Get and sApi.Get() or smoothingValue
            sApi.OnChange = function(v)
                smoothingValue = tonumber(v) or smoothingValue
                SetConfig("combat.aimbotSmoothing", smoothingValue)
            end
            pcall(function() sApi.Set(smoothingValue) end)
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
                                    local ok, lbl = pcall(function() return hrp:FindFirstChild("TeammateLabel") end)
                                    isTeammate = ok and lbl ~= nil
                                    teammateCache[pl] = { hrp = hrp, isTeam = isTeammate }
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
            if head and head.Position then
                pcall(function()
                    local predicted = head.Position
                    local root = head.Parent and (head.Parent:FindFirstChild("HumanoidRootPart") or head.Parent:FindFirstChild("Torso"))
                    if root and root:IsA("BasePart") then
                        local okDist, dist = pcall(function() return (head.Position - cam.CFrame.Position).Magnitude end)
                        local travel = projSpeed
                        if okDist and dist and travel and travel > 0 then
                            local tt = dist / travel
                            predicted = predicted + (root.Velocity or Vector3.new()) * tt * leadScale
                        end
                    end

                    local p = cam:WorldToViewportPoint(predicted)
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
                                    pcall(function() mousemoverel(toMoveX, toMoveY) end)
                                end
                            else
                                pcall(function() mousemoverel(dx, dy) end)
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
    local saved = GetConfig(KEY_CONFIG, "Q")
    local targetKey = (type(saved) == "string" and Enum.KeyCode[saved]) or Enum.KeyCode.Q

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
                
                activePlayers[playerName] = weaponName
                
                if not labels[playerName] then
                    labels[playerName] = createWeaponLabel(playerName)
                end
                
                local label = labels[playerName]
                label.Text = playerName .. " | " .. weaponName
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
        toggleAPI.OnToggle = function(state)
            onToggleChanged(state)
        end
    end
end

-- ** Show Enemy Weapons Logic Ends Here ** --

---------------------------------------------------------------------------

-- =================== Very end of Your Desire =================== --

-- ** Like a wise man once said, "Show me the client's state, and I'll show you the perfect hook." - some guy lol ** --
