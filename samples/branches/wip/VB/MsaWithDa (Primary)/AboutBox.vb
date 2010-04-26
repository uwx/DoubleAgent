Public NotInheritable Class AboutBox

    Public mDaControl As DoubleAgent.Control.AxControl

    Private Sub OKButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OKButton.Click
        Me.Close()
    End Sub

    Private Sub MoreButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MoreButton.Click
        mDaControl.PropertySheet.Visible = True
        mDaControl.PropertySheet.Page = "Copyright"
    End Sub
End Class
