function ShellOpenVB()
	dim lShellObject
    set lShellObject = CreateObject("Shell.Application")
    lShellObject.Open(Session.Property("ShellOpenPath"))
	set lShellObject = nothing
end function